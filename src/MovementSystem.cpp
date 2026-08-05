#include "MovementSystem.h"

bool MovementSystem::checkPointCollision(float tx, float ty, float curFloorH)
{
    if (raycaster.isWall(tx, ty)) return false;

    int sec = raycaster.getSectorAt((int)(tx / CELL_SIZE), (int)(ty / CELL_SIZE));
    float fH = raycaster.getFloorH(sec, tx, ty);
    float cH = raycaster.getCeilH(sec, tx, ty);

    if ((fH - curFloorH) > STEP_HEIGHT) return false;

    if ((cH - fH) < PLAYER_HEIGHT) return false;

    float highestFloor = (curFloorH > fH) ? curFloorH : fH;
    if (cH < (highestFloor + PLAYER_HEIGHT)) return false;

    return true;
}

bool MovementSystem::canMoveTo(float nx, float ny)
{
    auto& pos = coordinator->getComponent<Position>(raycaster.playerEntity);

    int curSec = raycaster.getPlayerSector();
    float curFloorH = raycaster.getFloorH(curSec, pos.x, pos.y);

    if (!checkPointCollision(nx, ny, curFloorH)) return false;
    if (!checkPointCollision(nx + MARGIN, ny, curFloorH)) return false;
    if (!checkPointCollision(nx - MARGIN, ny, curFloorH)) return false;
    if (!checkPointCollision(nx, ny + MARGIN, curFloorH)) return false;
    if (!checkPointCollision(nx, ny - MARGIN, curFloorH)) return false;

    return true;
}

void MovementSystem::update(float dt)
{
    auto& pos = coordinator->getComponent<Position>(raycaster.playerEntity);

    if (GetKey(KEY_UP).k_Held) pos.lookY += LOOK_SPEED;
    if (GetKey(KEY_DOWN).k_Held) pos.lookY -= LOOK_SPEED;
    pos.lookY = fmaxf(-MAX_LOOK, fminf(MAX_LOOK, pos.lookY));

    if (GetKey(KEY_A).k_Held)  pos.angle -= ROTATION_SPEED;
    if (GetKey(KEY_D).k_Held) pos.angle += ROTATION_SPEED;

    if (GetKey(KEY_W).k_Held)
    {
        float nx = pos.x + cosf(pos.angle * (float)DEG2RAD) * MOVEMENT_SPEED;
        float ny = pos.y + sinf(pos.angle * (float)DEG2RAD) * MOVEMENT_SPEED;
        if (canMoveTo(nx, pos.y)) pos.x = nx;
        if (canMoveTo(pos.x, ny)) pos.y = ny;
    }

    if (GetKey(KEY_S).k_Held)
    {
        float nx = pos.x - cosf(pos.angle * (float)DEG2RAD) * MOVEMENT_SPEED;
        float ny = pos.y - sinf(pos.angle * (float)DEG2RAD) * MOVEMENT_SPEED;
        if (canMoveTo(nx, pos.y)) pos.x = nx;
        if (canMoveTo(pos.x, ny)) pos.y = ny;
    }

    int pGridX = (int)(pos.x / CELL_SIZE);
    int pGridY = (int)(pos.y / CELL_SIZE);

    if (pGridX >= 0 && pGridX < MAP_WIDTH && pGridY >= 0 && pGridY < MAP_HEIGHT)
    {
        if (map[pGridY][pGridX] == PORTAL_TILE_ID)
        {
            int destGridX = 8;
            int destGridY = 16;
            float angleDelta = 180.0f;

            if (pGridX == 8 && pGridY == 16)
            {
                destGridX = 1;
                destGridY = 16;
            }

            // Extract position offset inside the grid square cell
            float relX = fmodf(pos.x, (float)CELL_SIZE);
            float relY = fmodf(pos.y, (float)CELL_SIZE);

            // Execute Teleportation warp
            pos.x = (destGridX * CELL_SIZE) + relX;
            pos.y = (destGridY * CELL_SIZE) + relY;
            pos.angle += angleDelta;

            // Step slightly out outward from the exit frame layout to safely skip re-trigger loops
            pos.x += cosf(pos.angle * (float)DEG2RAD) * (MARGIN + 1.0f);
            pos.y += sinf(pos.angle * (float)DEG2RAD) * (MARGIN + 1.0f);
        }
    }
}

void MovementSystem::setRaycasterData(Entity playerEnt, Coordinator* coord)
{
    raycaster.playerEntity = playerEnt;
    raycaster.setCoordinator(coord);  
}