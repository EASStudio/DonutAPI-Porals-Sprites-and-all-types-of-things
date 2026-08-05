#pragma once

#include <iostream>

namespace Screen 
{
    const int Width = 210;
    const int Height = 100; 
    const int Font_W = 8;
    const int Font_H = 8;
    const int FPS = 60;

    // Max we can do is 10,000 x 10,000 which is 100 megabytes and 5,000,000 cells, or 1,000 x 1,000 which is 1 megabytes and 50,000 cells
    // At 10,000 x 10,000 ( Which is 250,000 cells ) with 1,000,000 entitys it takes 2:49.51 to load and runs at a cool 2 fps with rendering 110,000 mobs

    const int MAP_WIDTH = 1000;
    const int MAP_HEIGHT = 1000; 
    const int CELL_SIZE = 20;

    const int FOV = 80;
    const int RENDER_DISTANCE = 250;
    const int VERTICAL_SCALE = Height * 50;
    const float LOOK_SPEED = 3.0f;
    const float MAX_LOOK = (float)(Height / 2 - 10);
    const double RAY_STEP = 0.1;
    const float ROTATION_SPEED = 1.0f;
    const float MOVEMENT_SPEED = 1.0f;
    const float MARGIN = 5.0f;
    const float STEP_HEIGHT = 0.55f;
    const float PLAYER_HEIGHT = 0.6f;
    const float COLLISION_RADIUS = 0.3f;

    const int SECTORS = 8;
    const int PORTAL_TILE_ID = 7;
    const int MAX_PORTAL_PASSES = 4;
    const int MAX_TEX = 50;
    const int MAX_MOBS_PER_DUNGEON = 8;

    const uint32_t MAX_DUNGEON_SIZE = 40;
}