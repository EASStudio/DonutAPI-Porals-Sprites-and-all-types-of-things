#include "Mob.h"
#include "EntitySystems.h"
#include "Globals.h"
#include "DonutAPI.h"
#include "DonutMath.h"

using namespace EntitySystems;
using namespace Screen;

void Mob::update(float dt)
{
    if (raycaster.playerEntity == INVALID_ENTITY) return;

    for (auto const& entity : entities)
    {
        if (entity == raycaster.playerEntity) continue;

        // Add safety checks
        if (!coordinator->hasComponent<Position>(entity) ||
            !coordinator->hasComponent<AIState>(entity)) {
            continue;
        }

        updateAI(entity, dt);
    }
}

void Mob::setPlayerEntity(Entity player)
{
    raycaster.playerEntity = player;
    if (coordinator)
    {
        raycaster.setCoordinator(coordinator);
        InternalLog("Mob system linked to playerEntity = " + std::to_string(player));
    }
}

void Mob::draw(const Textures::TextureSet& texSet)
{
    if (!coordinator)
    {
        InternalLog("Mob::draw skipped - no coordinator");
        return;
    }

    if (raycaster.playerEntity == INVALID_ENTITY)  
    {
        InternalLog("Mob::draw skipped - no playerEntity");
        return;
    }

    std::vector<SpriteDrawInfo> spritesToDraw;
    auto& playerPos = coordinator->getComponent<Position>(raycaster.playerEntity);

    for (auto const& entity : entities)
    {
        if (entity == raycaster.playerEntity) continue;

        auto& pos = coordinator->getComponent<Position>(entity);
        float dx = pos.x - playerPos.x;
        float dy = pos.y - playerPos.y;
        float distSq = dx * dx + dy * dy;

        if (distSq > 1000000.0f) continue;

        spritesToDraw.push_back({ entity, distSq });
    }

    InternalLog("Rendering " + std::to_string(spritesToDraw.size()) + " mob sprites");
    InternalLog("zBufferPtr is " + std::string(zBufferPtr ? "valid" : "null"));

    std::sort(spritesToDraw.begin(), spritesToDraw.end(),
        [](const SpriteDrawInfo& a, const SpriteDrawInfo& b) { return a.distance > b.distance; });

    for (const auto& info : spritesToDraw)
    {
        drawSprite(info.entity, texSet);
    }
}

int Mob::createMob(Coordinator& coord, float x, float y, int textureIndex)
{
    Entity mob = coord.createEntity();

    bool hostile = (textureIndex == 31 
        || textureIndex == 32 
        || textureIndex == 33 
        || textureIndex == 34 
        || textureIndex == 35 
        || textureIndex == 36 
        || textureIndex == 37);

    coord.addComponent<Position>(mob, Position{ x, y });
    coord.addComponent<MobSprite>(mob, MobSprite{ textureIndex, 1.0f });

    AIState ai{};
    ai.hostile = hostile;
    ai.state = AIState::WONDER;
    coord.addComponent<AIState>(mob, ai);

    if (hostile)
        coord.addComponent<Attack>(mob, Attack{ 8 }); // base hostile mob damage per hit

    return mob;
}

void Mob::drawSprite(Entity entity, const Textures::TextureSet& texSet)
{
    auto& playerPos = coordinator->getComponent<Position>(raycaster.playerEntity);
    auto& playerDir = coordinator->getComponent<Direction>(raycaster.playerEntity);
    auto& pos = coordinator->getComponent<Position>(entity);
    auto& sprite = coordinator->getComponent<MobSprite>(entity);

    int screenWidth = GetScreenWidth();

    float spriteX = pos.x - playerPos.x;
    float spriteY = pos.y - playerPos.y;

    float invDet = 1.0f / (playerDir.planeX * playerDir.dirY - playerDir.dirX * playerDir.planeY);
    float transformX = invDet * (playerDir.dirY * spriteX - playerDir.dirX * spriteY);
    float transformY = invDet * (-playerDir.planeY * spriteX + playerDir.planeX * spriteY);

    if (transformY <= 0.1f) return;

    int spriteScreenX = (int)((screenWidth * 0.5f) * (1.0f + transformX / transformY));

    // Ground the sprite to the actual floor, same math the raycaster uses for walls 
    int pSec = raycaster.getPlayerSector();
    float eyeH = raycaster.getFloorH(pSec, playerPos.x, playerPos.y) + 0.5f;

    int mobSec = raycaster.getSectorAt((int)(pos.x / CELL_SIZE), (int)(pos.y / CELL_SIZE));
    float mobFloorH = raycaster.getFloorH(mobSec, pos.x, pos.y);
    float mobTopH = mobFloorH + 1.0f * sprite.scale; // mob height in world units

    int topY = (int)(Height / 2.0 + playerPos.lookY + (eyeH - mobTopH) * ((double)VERTICAL_SCALE / transformY));
    int botY = (int)(Height / 2.0 + playerPos.lookY + (eyeH - mobFloorH) * ((double)VERTICAL_SCALE / transformY));

    int spriteHeight = botY - topY;
    if (spriteHeight <= 0) return;

    int spriteWidth = spriteHeight;

    int drawStartY = std::max(0, topY);
    int drawEndY = std::min(Height - 1, botY);

    int drawStartX = std::max(0, -spriteWidth / 2 + spriteScreenX);
    int drawEndX = std::min(screenWidth - 1, spriteWidth / 2 + spriteScreenX);

    if (sprite.textureIndex < 0 || sprite.textureIndex >= MAX_TEX) return;
    const Texture& tex = texSet.tex[sprite.textureIndex];

    for (int x = drawStartX; x <= drawEndX; x++)
    {
        if (zBufferPtr && transformY >= zBufferPtr[x]) continue;

        int texX = (int)(256 * (x - (-spriteWidth / 2 + spriteScreenX)) * 16 / spriteWidth) / 256;
        texX = std::clamp(texX, 0, 15);

        for (int y = drawStartY; y <= drawEndY; y++)
        {
            int texY = (int)(256 * (y - topY) * 16 / spriteHeight) / 256;
            texY = std::clamp(texY, 0, 15);

            unsigned short color = GetTexPixel(tex, texX, texY);
            if (color == 0 || color == COLOR::BLACK) continue;

            DrawPixel(x, y, PIXEL_SOLID, color);
        }
    }
}

void Mob::updateAI(Entity entity, float dt)
{
    auto& pos = coordinator->getComponent<Position>(entity);
    auto& ai = coordinator->getComponent<AIState>(entity);
    auto& playerPos = coordinator->getComponent<Position>(raycaster.playerEntity);

    float dx = playerPos.x - pos.x;
    float dy = playerPos.y - pos.y;
    float distSq = dx * dx + dy * dy;

    if (ai.attackCooldown > 0.0f)
        ai.attackCooldown -= dt;

    const float ATTACK_RANGE_SQ = 400.25f;  // ~40 units
    const float AGGRO_RANGE_SQ = 20000.0f; // ~141 units - hostile mobs notice/chase within this
    const float FLEE_RANGE_SQ = 5000.0f;  // ~70 units  - passive mobs bolt if you get this close

    if (ai.hostile)
    {
        if (distSq <= ATTACK_RANGE_SQ)      ai.state = AIState::ATTACK;
        else if (distSq <= AGGRO_RANGE_SQ)  ai.state = AIState::CHASE;
        else                                ai.state = AIState::WONDER;
    }

    else
    {
        ai.state = (distSq <= FLEE_RANGE_SQ) ? AIState::FLEE : AIState::WONDER;
    }

    float chaseSpeed = 30.0f * dt;
    float wanderSpeed = 12.0f * dt;
    float fleeSpeed = 28.0f * dt;

    switch (ai.state)
    {
    case AIState::CHASE:
        moveTowards(entity, playerPos.x, playerPos.y, chaseSpeed);
        break;

    case AIState::ATTACK:
        if (ai.attackCooldown <= 0.0f)
        {
            if (coordinator->hasComponent<Attack>(entity) &&
                coordinator->hasComponent<Health>(raycaster.playerEntity))
            {
                auto& atk = coordinator->getComponent<Attack>(entity);
                auto& hp = coordinator->getComponent<Health>(raycaster.playerEntity);
                hp.currentHp = (hp.currentHp > atk.value) ? hp.currentHp - atk.value : 0;
                InternalLog("Player hit for " + std::to_string(atk.value) + " dmg, hp now " + std::to_string(hp.currentHp));
            }
            ai.attackCooldown = 1.0f;
        }
        break;

    case AIState::FLEE:
        ai.wanderTimer -= dt;
        if (ai.wanderTimer <= 0.0f) pickWanderTarget(entity, true);
        moveAway(entity, ai.wanderTargetX, ai.wanderTargetY, fleeSpeed);
        break;

    case AIState::WONDER:
        ai.wanderTimer -= dt;
        if (ai.wanderTimer <= 0.0f) pickWanderTarget(entity, false);
        wonder(entity, ai.wanderTargetX, ai.wanderTargetY, wanderSpeed);
        break;

    case AIState::IDLE:
    default:
        break;
    }
}

bool Mob::moveTowards(Entity entity, float targetX, float targetY, float speed)
{
    auto& pos = coordinator->getComponent<Position>(entity);

    float dx = targetX - pos.x;
    float dy = targetY - pos.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 0.5f) return true;

    // Normalized movement vector
    float moveX = (dx / dist) * speed;
    float moveY = (dy / dist) * speed;

    float newX = pos.x + moveX;
    float newY = pos.y + moveY;

    // Check collision with walls
    auto checkWall = [this](float x, float y, float radius) -> bool
        {
            // Check all four corners of bounding box
            return raycaster.isWall(x - radius, y - radius) ||
                raycaster.isWall(x + radius, y - radius) ||
                raycaster.isWall(x - radius, y + radius) ||
                raycaster.isWall(x + radius, y + radius);
        };

    // Try full movement
    if (!checkWall(newX, newY, COLLISION_RADIUS))
    {
        pos.x = newX;
        pos.y = newY;
        return true;
    }

    // Wall sliding: try X-only
    if (!checkWall(newX, pos.y, COLLISION_RADIUS))
    {
        pos.x = newX;
        return true;
    }

    // Wall sliding: try Y-only
    if (!checkWall(pos.x, newY, COLLISION_RADIUS))
    {
        pos.y = newY;
        return true;
    }

    // Completely blocked
    return false;
}

bool Mob::moveAway(Entity entity, float targetX, float targetY, float speed)
{
    auto& pos = coordinator->getComponent<Position>(entity);

    float dx = targetX - pos.x;
    float dy = targetY - pos.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 0.5f) return true;

    float moveX = (dx / dist) * speed;
    float moveY = (dy / dist) * speed;

    float newX = pos.x + moveX;
    float newY = pos.y + moveY;

    // Check collision with walls
    auto checkWall = [this](float x, float y, float radius) -> bool
        {
            // Check all four corners of bounding box
            return raycaster.isWall(x - radius, y - radius) ||
                raycaster.isWall(x + radius, y - radius) ||
                raycaster.isWall(x - radius, y + radius) ||
                raycaster.isWall(x + radius, y + radius);
        };

    // Try full movement
    if (!checkWall(newX, newY, COLLISION_RADIUS))
    {
        pos.x = newX;
        pos.y = newY;
        return true;
    }

    // Wall sliding: try X-only
    if (!checkWall(newX, pos.y, COLLISION_RADIUS))
    {
        pos.x = newX;
        return true;
    }

    // Wall sliding: try Y-only
    if (!checkWall(pos.x, newY, COLLISION_RADIUS))
    {
        pos.y = newY;
        return true;
    }

    // Completely blocked
    return false;
}

bool Mob::wonder(Entity entity, float targetX, float targetY, float speed)
{
    auto& pos = coordinator->getComponent<Position>(entity);

    float dx = targetX - pos.x;
    float dy = targetY - pos.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 0.5f) return true;

    float moveX = (dx / dist) * speed;
    float moveY = (dy / dist) * speed;

    float newX = pos.x + moveX;
    float newY = pos.y + moveY;

    // Check collision with walls
    auto checkWall = [this](float x, float y, float radius) -> bool
        {
            // Check all four corners of bounding box
            return raycaster.isWall(x - radius, y - radius) ||
                raycaster.isWall(x + radius, y - radius) ||
                raycaster.isWall(x - radius, y + radius) ||
                raycaster.isWall(x + radius, y + radius);
        };

    // Try full movement
    if (!checkWall(newX, newY, COLLISION_RADIUS))
    {
        pos.x = newX;
        pos.y = newY;
        return true;
    }

    // Wall sliding: try X-only
    if (!checkWall(newX, pos.y, COLLISION_RADIUS))
    {
        pos.x = newX;
        return true;
    }

    // Wall sliding: try Y-only
    if (!checkWall(pos.x, newY, COLLISION_RADIUS))
    {
        pos.y = newY;
        return true;
    }

    // Completely blocked
    return false;
}

void Mob::pickWanderTarget(Entity entity, bool fleeFromPlayer)
{
    auto& pos = coordinator->getComponent<Position>(entity);
    auto& ai = coordinator->getComponent<AIState>(entity);

    if (fleeFromPlayer)
    {
        auto& playerPos = coordinator->getComponent<Position>(raycaster.playerEntity);

        float awayX = pos.x - playerPos.x;
        float awayY = pos.y - playerPos.y;
        float len = sqrtf(awayX * awayX + awayY * awayY);
        if (len < 0.01f) { awayX = 1.0f; awayY = 0.0f; len = 1.0f; }
        awayX /= len; awayY /= len;

        float fleeDist = 6.0f * CELL_SIZE;
        ai.wanderTargetX = pos.x + awayX * fleeDist;
        ai.wanderTargetY = pos.y + awayY * fleeDist;
    }

    else
    {
        float radius = 5.0f * CELL_SIZE;
        float angle = GetRandomValue(0, 359) * DEG2RAD;
        float dist = (float)GetRandomValue(2, (int)radius);
        ai.wanderTargetX = pos.x + cosf(angle) * dist;
        ai.wanderTargetY = pos.y + sinf(angle) * dist;
    }

    ai.wanderTimer = 3.0f + GetRandomValue(0, 300) / 100.0f; 
}
