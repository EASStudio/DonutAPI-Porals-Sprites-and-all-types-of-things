#include "Game.h"

using namespace Screen;
using namespace EntitySystems;

// Helper functions

static void syncDirectionFromAngle(Coordinator& coord, Entity player) 
{
    auto& pos = coord.getComponent<Position>(player);
    auto& dir = coord.getComponent<Direction>(player);

    float rad = pos.angle * DEG2RAD;
    dir.dirX = cosf(rad);
    dir.dirY = sinf(rad);

    // Camera plane (perpendicular, scaled for FOV)
    float planeScale = tanf((FOV * 0.5f) * DEG2RAD);
    dir.planeX = -dir.dirY * planeScale;
    dir.planeY = dir.dirX * planeScale;
}

Game::Game() :
    window(Width, Height, Font_W, Font_H, FPS)
{

}

void Game::Shutdown()
{
    appIsRunning = false;
    window.CloseWindow();           
}

void Game::Run()
{
    coordinator.init();

    // Register all components
    coordinator.registerComponent<Position>();
    coordinator.registerComponent<Health>();
    coordinator.registerComponent<Attack>();
    coordinator.registerComponent<Defense>();
    coordinator.registerComponent<Mana>();
    coordinator.registerComponent<Weapon>();
    coordinator.registerComponent<Armor>();
    coordinator.registerComponent<Direction>();
    coordinator.registerComponent<MobSprite>();
    coordinator.registerComponent<AIState>();
    coordinator.registerComponent<PlayerTag>();

    // Register systems
    player = coordinator.registerSystem<Player>();
    player->coordinator = &coordinator;

    auto combatSystem = coordinator.registerSystem<CombatSystem>();
    combatSystem->coordinator = &coordinator;

    auto mobSystem = coordinator.registerSystem<Mob>();
    mobSystem->coordinator = &coordinator;

    auto itemSystem = coordinator.registerSystem<Item>();
    itemSystem->coordinator = &coordinator;

    auto movementSystem = coordinator.registerSystem<MovementSystem>();
    movementSystem->coordinator = &coordinator;

    // Set signatures
    Signature playerSig;
    playerSig.set(coordinator.getComponentType<Position>(), true);
    coordinator.setSystemSignature<Player>(playerSig);

    Signature mobSignature;
    mobSignature.set(coordinator.getComponentType<Position>(), true);
    mobSignature.set(coordinator.getComponentType<MobSprite>(), true);
    mobSignature.set(coordinator.getComponentType<AIState>(), true);
    coordinator.setSystemSignature<Mob>(mobSignature);

    // Create player
    raycaster.playerEntity = coordinator.createEntity();
    coordinator.addComponent(raycaster.playerEntity, Position{});
    coordinator.addComponent(raycaster.playerEntity, Health{ 100, 100 });
    coordinator.addComponent(raycaster.playerEntity, Attack{ 12 });
    coordinator.addComponent(raycaster.playerEntity, Defense{ 5 });
    coordinator.addComponent(raycaster.playerEntity, Mana{ 5 });
    coordinator.addComponent(raycaster.playerEntity, PlayerTag{});
    coordinator.addComponent(raycaster.playerEntity, Direction{});

    raycaster.setCoordinator(&coordinator);

    // Setup movement system
    movementSystem->setRaycasterData(raycaster.playerEntity, &coordinator);

    // Setup mob system
    mobSystem->setPlayerEntity(raycaster.playerEntity);
    mobSystem->setZBuffer(raycaster.zBuffer);  

    // Setup mob system's internal raycaster
    mobSystem->setCoordinator(&coordinator); 

    // Setup other systems
    texture.createTextures();
    perlin.initWorld();
    mapEditor.loadMap();
    perlin.spawnMobsFromMap(*mobSystem, coordinator);
    music.buildMusic();       
    music.playMainTheme();

    // Check if mobs were created
    std::string str = "Spawned " + std::to_string(perlin.getMobSpawns().size()) + " mobs";
    InternalLog(str);

    // Check mob system entities
    InternalLog("Mob system has " + std::to_string(mobSystem->entities.size()) + " entities");

    bool gameIsRunning = false;
    bool aboutIsRunning = false;

    while (!WindowShouldClose())
    {
        Fill(0, 0, Width, Height, PIXEL_SOLID, BLACK);

        GetKeyState();

        if (mapEditor.editor.enabled)
        {
            mapEditor.editorInput();
            mapEditor.drawEditor();
        }

        else if (gameIsRunning)
        {
            float dt = GetElapsedTime();

            bool isMoving = GetKey(KEY_W).k_Held || GetKey(KEY_S).k_Held || GetKey(KEY_A).k_Held || GetKey(KEY_D).k_Held;

            if (GetKey(KEY_M).k_Pressed)
                music.toggleMusic();

            player->update(dt);
            combatSystem->update(dt);
            mobSystem->update(dt);
            itemSystem->update(dt);
            movementSystem->update(dt);

            raycaster.drawBackground(texture.texSet, 13);
            raycaster.drawFOV(texture.texSet, 18, 7);

            if (isMoving)
                dt *= 3.0f; // Faster bob when moving

            raycaster.drawArms(texture.texSet, dt);
            syncDirectionFromAngle(coordinator, raycaster.playerEntity);

            mobSystem->draw(texture.texSet);

            if (GetKey(KEY_TAB).k_Pressed) mapEditor.editor.enabled = true;
        }

        else if (aboutIsRunning)
        {
            DrawString(106, 23, L"About", WHITE);

            if (GetKey(KEY_ESC).k_Pressed) aboutIsRunning = false;
        }

        else
        {
            DrawString(100, 23, L"Welcome to Quest", RED);
            DrawString(100, 24, L"1. Campaign", RED);
            DrawString(100, 25, L"2. About", RED);
            DrawString(100, 26, L"3. Quit", RED);

            if (GetKey(KEY_TAB).k_Pressed) mapEditor.editor.enabled = true;
            if (GetKey(KEY_1).k_Pressed) gameIsRunning = true;
            if (GetKey(KEY_2).k_Pressed) aboutIsRunning = true;
            if (GetKey(KEY_3).k_Pressed) Shutdown();
        }

        UpdateScreen();
    }

    Shutdown();
}