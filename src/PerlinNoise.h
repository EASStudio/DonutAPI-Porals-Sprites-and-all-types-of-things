#pragma once

#include "WFC.h"
#include "Textures.h"
#include "Coordinator.h"
#include "Mob.h"
#include <vector>

struct MobSpawn
{
	float x;
	float y;
	int textureIndex;
};

class PerlinNoise 
{
public:
	PerlinNoise();
	void initWorld();
	void spawnMobsFromMap(Mob& mobSystem, Coordinator& coord);

	const std::vector<MobSpawn>& getMobSpawns() const { return mobSpawns; }

private:
	WFC wfc;
	std::vector<MobSpawn> mobSpawns;

	void placeDungeons();
	void placeMobsInTerrain();
	void placeMobsInDungeon(int worldX, int worldY, int dungeonW, int dungeonH);
};