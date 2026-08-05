#include "DonutMath.h"

#include "Raycaster.h" 
#include "EntitySystems.h"

using namespace EntitySystems;

int map[MAP_HEIGHT][MAP_WIDTH];
int sectorMap[MAP_HEIGHT][MAP_WIDTH];

Raycaster::SectorDef sectors[SECTORS] =
{
    { 0.00f, 1.00f,  0.00f, 0.00f,  0.00f, 0.00f,  13, 0 },  // 0: Default floor
    { 0.10f, 1.00f,  0.00f, 0.00f,  0.00f, 0.00f,  13, 0 },
    { 0.20f, 1.00f,  0.05f, 0.05f,  0.00f, 0.00f,  14, 0 },  // Gravel areas
    { 0.00f, 1.00f,  0.00f, 0.00f,  0.00f, 0.00f,  5,  0 },  // Stone
    { 0.00f, 1.00f,  0.00f, 0.00f,  0.00f, 0.00f,  13, 0 },
    { 0.00f, 0.80f,  0.00f, 0.00f,  0.00f, 0.00f,  13, 7 },  // Snow biome lower ceiling feel
    { 0.15f, 1.00f,  0.10f, -0.05f, 0.00f, 0.00f,  13, 0 },
    { 0.00f, 1.00f,  0.00f, 0.00f,  0.00f, 0.00f,  3,  0 }   // Mountain stone
};

int Raycaster::getSectorAt(int cellX, int cellY)
{
    if (cellX < 0 || cellX >= MAP_WIDTH || cellY < 0 || cellY >= MAP_HEIGHT) return 0;
    return sectorMap[cellY][cellX];
}

int Raycaster::getPlayerSector()
{
    auto& pos = coordinator->getComponent<Position>(playerEntity);
    return getSectorAt((int)(pos.x / CELL_SIZE), (int)(pos.y / CELL_SIZE));
}

float Raycaster::getFloorH(int secId, float wx, float wy)
{
    if (secId < 0 || secId >= SECTORS) return 0.0f;
    const Raycaster::SectorDef& s = sectors[secId];
    float cx = fmodf(wx, (float)CELL_SIZE) / CELL_SIZE; if (cx < 0.f) cx += 1.f;
    float cy = fmodf(wy, (float)CELL_SIZE) / CELL_SIZE; if (cy < 0.f) cy += 1.f;
    return s.floorH + s.floorSlopeX * cx + s.floorSlopeY * cy;
}

float Raycaster::getCeilH(int secId, float wx, float wy)
{
    if (secId < 0 || secId >= SECTORS) return 1.0f;
    const Raycaster::SectorDef& s = sectors[secId];
    float cx = fmodf(wx, (float)CELL_SIZE) / CELL_SIZE; if (cx < 0.f) cx += 1.f;
    float cy = fmodf(wy, (float)CELL_SIZE) / CELL_SIZE; if (cy < 0.f) cy += 1.f;
    return s.ceilH + s.ceilSlopeX * cx + s.ceilSlopeY * cy;
}

float Raycaster::getFloorHCentre(int sec, int cx, int cy)
{
    return Raycaster::getFloorH(sec, (cx + 0.5f) * CELL_SIZE, (cy + 0.5f) * CELL_SIZE);
}

float Raycaster::getCeilHCentre(int sec, int cx, int cy)
{
    return Raycaster::getCeilH(sec, (cx + 0.5f) * CELL_SIZE, (cy + 0.5f) * CELL_SIZE);
}

int Raycaster::projectY(float worldH, double dist, float eyeH)
{
    if (dist <= 0.0) return 0;
    auto& pos = coordinator->getComponent<Position>(playerEntity);
    return (int)(Height / 2.0 + pos.lookY + (eyeH - worldH) * ((double)VERTICAL_SCALE / dist));
}

Raycaster::Ray Raycaster::castRay(double angle)
{
    auto& pos = coordinator->getComponent<Position>(playerEntity);
    double rad = angle * DEG2RAD;
    double rayX = pos.x;
    double rayY = pos.y;
    int lastOpenCX = (int)(pos.x / CELL_SIZE);
    int lastOpenCY = (int)(pos.y / CELL_SIZE);

    while (true)
    {
        rayX += cos(rad) * RAY_STEP; rayY += sin(rad) * RAY_STEP;
        double dist = sqrt((rayX - pos.x) * (rayX - pos.x) + (rayY - pos.y) * (rayY - pos.y));

        if (dist > RENDER_DISTANCE)
            return { (double)RENDER_DISTANCE, 0, 0.f, (float)rayX, (float)rayY, 0, 0, getSectorAt(lastOpenCX, lastOpenCY) };

        int cellX = (int)rayX / CELL_SIZE, cellY = (int)rayY / CELL_SIZE;

        if (cellX < 0 || cellX >= MAP_WIDTH || cellY < 0 || cellY >= MAP_HEIGHT)
            return { (double)RENDER_DISTANCE, 0, 0.f, (float)rayX, (float)rayY, cellX, cellY, getSectorAt(lastOpenCX, lastOpenCY) };

        int cell = map[cellY][cellX];

        if (cell == 0)
        {
            lastOpenCX = cellX;
            lastOpenCY = cellY;
        }

        else
        {
            float fx = fmod((float)rayX, (float)CELL_SIZE) / CELL_SIZE;
            float fy = fmod((float)rayY, (float)CELL_SIZE) / CELL_SIZE;
            float wallX = (fabs(fx - 0.5f) > fabs(fy - 0.5f)) ? fy : fx;
            return { dist, cell, wallX, (float)rayX, (float)rayY, cellX, cellY, getSectorAt(lastOpenCX, lastOpenCY) };
        }
    }
    return { (double)RENDER_DISTANCE, 0, 0.f, 0.f, 0.f, 0, 0, 0 };
}

bool Raycaster::isWall(float x, float y)
{
    int cx = (int)x / CELL_SIZE, cy = (int)y / CELL_SIZE;
    if (cx < 0 || cx >= MAP_WIDTH || cy < 0 || cy >= MAP_HEIGHT) return true;

    if (map[cy][cx] == PORTAL_TILE_ID) return false;

    return map[cy][cx] != 0;
}

static void drawVStrip(int screenX, int yTop, int yBottom, float texU, const Texture& tex)
{
    int totalPx = yBottom - yTop;

    if (totalPx <= 0) return;

    int dt = (yTop < 0) ? 0 : yTop;
    int db = (yBottom >= Height) ? Height - 1 : yBottom;

    for (int y = dt; y <= db; y++)
    {
        float v = (float)(y - yTop) / (float)totalPx;
        unsigned short col = SampleTexture(tex, texU, v);
        if (col != 0 && col != BLACK)  // Ignore pure black as transparent
            DrawPixel(screenX, y, PIXEL_SOLID, col);
    }
}

void Raycaster::drawBackground(const Textures::TextureSet& texSet, int floorID)
{
    auto& pos = coordinator->getComponent<Position>(playerEntity);
    int pSec = getPlayerSector();
    float eyeH = getFloorH(pSec, pos.x, pos.y) + 0.5f;

    float leftAngle = (pos.angle - FOV / 2.0f) * (float)DEG2RAD;
    float rightAngle = (pos.angle + FOV / 2.0f) * (float)DEG2RAD;
    float rdX0 = cosf(leftAngle), rdY0 = sinf(leftAngle);
    float rdX1 = cosf(rightAngle), rdY1 = sinf(rightAngle);

    for (int y = 0; y < Height; y++)
    {
        float dy = (float)y - (Height / 2.0f + pos.lookY);
        if (fabsf(dy) < 0.5f) continue;

        bool  isFloor = (dy > 0.0f);
        float eyeOffset = isFloor ? eyeH : (1.0f - eyeH);
        if (eyeOffset <= 0.0f) continue;

        float rowDist = eyeOffset * (float)VERTICAL_SCALE / fabsf(dy);

        float wx = pos.x + rowDist * rdX0;
        float wy = pos.y + rowDist * rdY0;
        float sx = rowDist * (rdX1 - rdX0) / (float)Width;
        float sy = rowDist * (rdY1 - rdY0) / (float)Width;

        for (int x = 0; x < Width; x++, wx += sx, wy += sy)
        {
            int sec = getSectorAt((int)(wx / CELL_SIZE), (int)(wy / CELL_SIZE));

            float fh = isFloor ? getFloorH(sec, wx, wy) : getCeilH(sec, wx, wy);
            float actualOffset = isFloor ? (eyeH - fh) : (fh - eyeH);
            if (actualOffset <= 0.0f) continue;

            float corrDist = actualOffset * (float)VERTICAL_SCALE / fabsf(dy);
            float scale = corrDist / rowDist;

            float wxS = pos.x + (wx - pos.x) * scale;
            float wyS = pos.y + (wy - pos.y) * scale;

            int secS = getSectorAt((int)(wxS / CELL_SIZE), (int)(wyS / CELL_SIZE));
            float u = fmodf(wxS / CELL_SIZE, 1.0f); if (u < 0.f) u += 1.f;
            float v = fmodf(wyS / CELL_SIZE, 1.0f); if (v < 0.f) v += 1.f;

            int texId = isFloor ? sectors[secS].floorTexId : sectors[secS].ceilTexId;

            if (texId > 0 && texId < MAX_TEX && &texSet.tex[texId])
                DrawPixel(x, y, PIXEL_SOLID, SampleTexture(texSet.tex[floorID], u, v));
            else
                DrawPixel(x, y, isFloor ? PIXEL_QUARTER : PIXEL_SOLID, isFloor ? DARK_GREEN : DARK_BLUE);
        }
    }
}

void Raycaster::drawFOV(const Textures::TextureSet& texSet, int slopeID, int extWallID)
{
    auto& pos = coordinator->getComponent<Position>(playerEntity);
    std::fill(zBuffer, zBuffer + Width, 1e30);

    int pSec = getPlayerSector();
    float eyeH = getFloorH(pSec, pos.x, pos.y) + 0.5f;

    const Texture* riserTex = &texSet.tex[slopeID];   // slope texture

    for (int x = 0; x < Width; x++)
    {
        double initialAngle = (pos.angle - FOV / 2.0) + ((double)x / (double)Width) * FOV;
        double currentAngle = initialAngle;

        double rayDirX = cos(currentAngle * DEG2RAD);
        double rayDirY = sin(currentAngle * DEG2RAD);

        double rayPosX = pos.x / (double)CELL_SIZE;
        double rayPosY = pos.y / (double)CELL_SIZE;
        int mapX = (int)rayPosX, mapY = (int)rayPosY;

        double deltaDistX = (fabs(rayDirX) < 1e-20) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (fabs(rayDirY) < 1e-20) ? 1e30 : fabs(1.0 / rayDirY);
        double sideDistX;
        double sideDistY;
        int stepX;
        int stepY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
        }

        // State trackers across portal jumps
        double accumulatedDistance = 0.0;
        int portalPasses = 0;

        for (int ddaStep = 0; ddaStep < 200; ddaStep++)
        {
            int side;
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) break;

            double perpDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);

            // Calculate precision ray intersection hit metrics (in grid unit fractions)
            float hitX = (side == 0) ? (float)mapX : (float)(rayPosX + perpDist * rayDirX);
            float hitY = (side == 1) ? (float)mapY : (float)(rayPosY + perpDist * rayDirY);

            if (map[mapY][mapX] == PORTAL_TILE_ID && portalPasses < MAX_PORTAL_PASSES)
            {
                portalPasses++;
                accumulatedDistance += perpDist * CELL_SIZE;

                int destGridX = 8;
                int destGridY = 16;
                float angleDelta = 180.0f; // Turn perfectly backwards facing out of the target window

                if (mapX == 8 && mapY == 16)
                {
                    destGridX = 1;
                    destGridY = 16;
                }

                // Preserve alignment offset where the ray crossed the portal boundary line
                float relX = fmodf(hitX, 1.0f); if (relX < 0.0f) relX += 1.0f;
                float relY = fmodf(hitY, 1.0f); if (relY < 0.0f) relY += 1.0f;

                // Adjust ray angle
                currentAngle += angleDelta;
                rayDirX = cos(currentAngle * DEG2RAD);
                rayDirY = sin(currentAngle * DEG2RAD);

                // Re-calculate start origin just past destination plane to avoid looping
                rayPosX = (double)destGridX + relX + (rayDirX * 0.01);
                rayPosY = (double)destGridY + relY + (rayDirY * 0.01);
                mapX = (int)rayPosX;
                mapY = (int)rayPosY;

                // Refresh tracking data for the DDA loop vector space
                deltaDistX = (fabs(rayDirX) < 1e-20) ? 1e30 : fabs(1.0 / rayDirX);
                deltaDistY = (fabs(rayDirY) < 1e-20) ? 1e30 : fabs(1.0 / rayDirY);

                if (rayDirX < 0)
                {
                    stepX = -1;
                    sideDistX = (rayPosX - mapX) * deltaDistX;
                }
                else
                {
                    stepX = 1;
                    sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
                }

                if (rayDirY < 0)
                {
                    stepY = -1;
                    sideDistY = (rayPosY - mapY) * deltaDistY;
                }
                else
                {
                    stepY = 1;
                    sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
                }

                continue; // Continue ray path traversal seamlessly out of the target portal
            }

            // Total perspective distance combines tracks prior to porting
            double totalWorldDist = accumulatedDistance + (perpDist * CELL_SIZE);
            double worldDist = totalWorldDist * cos((initialAngle - pos.angle) * DEG2RAD);
            if (worldDist < 0.5) worldDist = 0.5;

            float worldHitX = hitX * CELL_SIZE;
            float worldHitY = hitY * CELL_SIZE;

            int prevCX = mapX - (side == 0 ? stepX : 0);
            int prevCY = mapY - (side == 1 ? stepY : 0);
            int prevSec = getSectorAt(prevCX, prevCY);
            int curSec = getSectorAt(mapX, mapY);

            float texU = (side == 0) ? fmodf(worldHitY / (float)CELL_SIZE, 1.0f)
                : fmodf(worldHitX / (float)CELL_SIZE, 1.0f);

            if (texU < 0.0f) texU += 1.0f;

            if (map[mapY][mapX] != 0)
            {
                float wallFloorH = getFloorH(prevSec, worldHitX, worldHitY);
                float wallCeilH = getCeilH(prevSec, worldHitX, worldHitY);
                int yBottom = projectY(wallFloorH, worldDist, eyeH);
                int yTop = projectY(wallCeilH, worldDist, eyeH);

                int wallTexId = map[mapY][mapX];
                if (wallTexId > 0 && wallTexId < MAX_TEX && &texSet.tex[wallTexId])
                    drawVStrip(x, yTop, yBottom, texU, texSet.tex[wallTexId]);
                else
                {
                    int dt = (yTop < 0) ? 0 : yTop;
                    int db = (yBottom >= Height) ? Height - 1 : yBottom;
                    for (int y = dt; y <= db; y++) DrawPixel(x, y, PIXEL_SOLID, DARK_GRAY);
                }

                zBuffer[x] = worldDist;   // Record distance for this column
                break;
            }

            float oldFloorH = getFloorHCentre(prevSec, prevCX, prevCY);
            float newFloorH = getFloorHCentre(curSec, mapX, mapY);
            float oldCeilH = getCeilHCentre(prevSec, prevCX, prevCY);
            float newCeilH = getCeilHCentre(curSec, mapX, mapY);

            const float T = 0.02f;

            if (newFloorH > oldFloorH + T && riserTex)
                drawVStrip(x, projectY(newFloorH, worldDist, eyeH),
                    projectY(oldFloorH, worldDist, eyeH), texU, *riserTex);

            if (newFloorH < oldFloorH - T && riserTex)
                drawVStrip(x, projectY(oldFloorH, worldDist, eyeH),
                    projectY(newFloorH, worldDist, eyeH), texU, *riserTex);

            if (newCeilH < oldCeilH - T)
            {
                int cTexId = sectors[curSec].ceilTexId;
                const Texture* cTex = (cTexId > 0 && cTexId < MAX_TEX) ? &texSet.tex[cTexId]
                    : &texSet.tex[7];
                if (cTex)
                    drawVStrip(x, projectY(oldCeilH, worldDist, eyeH),
                        projectY(newCeilH, worldDist, eyeH), texU, *cTex);
            }

            if (newCeilH > oldCeilH + T && &texSet.tex[extWallID])
                drawVStrip(x, projectY(newCeilH, worldDist, eyeH),
                    projectY(oldCeilH, worldDist, eyeH), texU, texSet.tex[extWallID]);
        }
    }
}

void Raycaster::drawArms(const Textures::TextureSet& texSet, float bob)
{
    int armW = 32;
    int armH = 48;

    // Left arm - bottom left
    int leftX = 20 + (int)(sinf(bob * 8.0f) * 3);           // bob
    int leftY = Height - armH - 8 + (int)(fabsf(sinf(bob * 4.0f)) * 6);

    // Right arm - bottom right (slightly overlapping or weapon hand)
    int rightX = Width - armW - 20 + (int)(sinf(bob * 8.0f + 3.14f) * 3);
    int rightY = Height - armH - 8 + (int)(fabsf(sinf(bob * 4.0f)) * 6);

    // Draw left arm
    for (int y = 0; y < armH; ++y)
    {
        for (int x = 0; x < armW; ++x)
        {
            unsigned short col = SampleTexture(texSet.tex[27], (float)x / armW, (float)y / armH);
            if (col != 0 && col != BLACK)
            {
                int screenX = leftX + x;
                int screenY = leftY + y;
                if (screenX >= 0 && screenX < Width && screenY >= 0 && screenY < Height)
                    DrawPixel(screenX, screenY, PIXEL_SOLID, col);
            }
        }
    }

    // Draw right arm
    for (int y = 0; y < armH; ++y)
    {
        for (int x = 0; x < armW; ++x)
        {
            unsigned short col = SampleTexture(texSet.tex[28], (float)x / armW, (float)y / armH);
            if (col != 0 && col != BLACK)
            {
                int screenX = rightX + x;
                int screenY = rightY + y;
                if (screenX >= 0 && screenX < Width && screenY >= 0 && screenY < Height)
                    DrawPixel(screenX, screenY, PIXEL_SOLID, col);
            }
        }
    }
}