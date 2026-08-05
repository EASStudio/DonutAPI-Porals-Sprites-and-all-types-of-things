#pragma once

#include "Window.h"
#include "MapEditor.h"
#include "Textures.h"
#include "PerlinNoise.h"
#include "Coordinator.h"
#include "CombatSystem.h"
#include "Player.h"
#include "Mob.h"
#include "Item.h"
#include "MovementSystem.h"
#include "Music.h"

class Game
{
public:
	Game();
	void Run();
	void Shutdown();

private:
	Window window;
	Raycaster raycaster;
	MapEditor mapEditor;
	Textures texture;
	PerlinNoise perlin;
	Coordinator coordinator;
	MusicSystem music;

	std::shared_ptr<Player> player;

private:
	bool appIsRunning = false;
};