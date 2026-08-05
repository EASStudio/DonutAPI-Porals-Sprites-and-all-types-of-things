#pragma once

#include "Textures.h"
#include "Coordinator.h"
#include <vector>

class Raycaster 
{
public:
    struct SectorDef
    {
        float floorH;
        float ceilH;
        float floorSlopeX;
        float floorSlopeY;
        float ceilSlopeX;
        float ceilSlopeY;
        int floorTexId;
        int ceilTexId;
    };

    struct Ray
    {
        double distance;
        int wallType;
        float wallX;
        float worldX;
        float worldY;
        int cellX;
        int cellY;
        int sectorId;
    };

    struct PortalPair 
    {
        Vector2 portal1;
        Vector2 portal2;
        double angleDelta;
    };

public:
	void drawFOV(const Textures::TextureSet& texSet, int slopeID, int extWallID);
	void drawBackground(const Textures::TextureSet& texSet, int floorID);
    void drawArms(const Textures::TextureSet& texSet, float bob);

    bool isWall(float x, float y);
    int getSectorAt(int cellX, int cellY);
    float getFloorH(int secId, float wx, float wy);
    float getCeilH(int secId, float wx, float wy);
    int getPlayerSector();
    float getFloorHCentre(int sec, int cx, int cy);
    float getCeilHCentre(int sec, int cx, int cy);
    void setCoordinator(Coordinator* coord) { coordinator = coord; }

public:
    Entity playerEntity = 0;
    double zBuffer[Width] = { 0 };

private:
    int projectY(float worldH, double dist, float eyeH);
    Ray castRay(double angle);

private:
    Textures texture;
    Coordinator* coordinator = nullptr;
};

extern Raycaster::SectorDef sectors[SECTORS];

extern int map[MAP_HEIGHT][MAP_WIDTH];
extern int sectorMap[MAP_HEIGHT][MAP_WIDTH];