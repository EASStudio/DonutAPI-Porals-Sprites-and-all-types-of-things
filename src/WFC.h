#pragma once

#include "Globals.h"

#include <vector>
#include <random>
#include <optional>

using namespace Screen;

class WFC
{
public:
    struct Tile
    {
        int id = 0;
        uint8_t sockets[4] = { 0,0,0,0 }; // N, E, S, W
        float weight = 1.0f;         
    };

    std::vector<Tile> tileSet;
    int generatedMap[MAX_DUNGEON_SIZE][MAX_DUNGEON_SIZE] = {};

public:
    WFC();
    bool generateDungeon(int width, int height, uint32_t seed = 0, int maxAttempts = 20);
    void placeInWorld(int worldX, int worldY);
    void renderMiniMap();

private:
    struct Cell
    {
        std::vector<int> possibleTiles;
        bool collapsed = false;
    };

    std::vector<std::vector<Cell>> grid;
    std::mt19937 rng;

    int lastWidth = 0;
    int lastHeight = 0;

private:
    void initTileSet();
    int getLowestEntropy(int& outX, int& outY);
    void collapse(int x, int y);
    void propagate(int sx, int sy);
    bool compatible(int tileA, int tileB, int dir) const;
    bool isGenerationValid() const;
};