#include "WFC.h"
#include "Raycaster.h"
#include "Globals.h"

#include <stack>
#include <algorithm>
#include <numeric>
#include <array>      
#include <utility>

WFC::WFC()
{
    rng.seed(std::random_device{}());
    initTileSet();
}

void WFC::initTileSet()
{
    tileSet.clear();

    // Format: {id, sockets, weight}
    tileSet.push_back({ 0, {0,0,0,0}, 5.0f });   // 0: Empty floor (common)

    tileSet.push_back({ 1, {1,1,1,1}, 3.0f });   // 1: Solid wall

    // Corridors
    tileSet.push_back({ 6, {0,1,0,1}, 2.0f });   // Horizontal
    tileSet.push_back({ 6, {1,0,1,0}, 2.0f });   // Vertical

    // Corners
    tileSet.push_back({ 1, {0,1,1,0}, 1.5f });
    tileSet.push_back({ 1, {1,0,0,1}, 1.5f });
    tileSet.push_back({ 1, {1,1,0,0}, 1.5f });
    tileSet.push_back({ 1, {0,0,1,1}, 1.5f });

    // T-junctions
    tileSet.push_back({ 1, {1,1,0,1}, 1.2f });
    tileSet.push_back({ 1, {1,1,1,0}, 1.2f });
    tileSet.push_back({ 1, {0,1,1,1}, 1.2f });
    tileSet.push_back({ 1, {1,0,1,1}, 1.2f });

    tileSet.push_back({ 1, {1,1,1,1}, 1.0f });   // Cross

    // Doors
    tileSet.push_back({ 7, {0,1,0,1}, 0.8f });
    tileSet.push_back({ 7, {1,0,1,0}, 0.8f });

    // Floor variations
    tileSet.push_back({ 14, {0,0,0,0}, 4.0f });  // Gravel
    // TODO: Add more: blood floor, decorated floor, 
}

bool WFC::compatible(int tileA, int tileB, int dir) const
{
    if (tileA < 0 || tileA >= (int)tileSet.size() ||
        tileB < 0 || tileB >= (int)tileSet.size())
        return false;

    int oppositeDir = (dir + 2) % 4;

    return tileSet[tileA].sockets[dir] == tileSet[tileB].sockets[oppositeDir];
}

int WFC::getLowestEntropy(int& outX, int& outY)
{
    int minEntropy = INT_MAX;
    outX = -1; outY = -1;

    for (int y = 0; y < (int)grid.size(); ++y)
    {
        for (int x = 0; x < (int)grid[y].size(); ++x)
        {
            if (grid[y][x].collapsed) continue;

            int entropy = (int)grid[y][x].possibleTiles.size();
            if (entropy > 0 && entropy < minEntropy)
            {
                minEntropy = entropy;
                outX = x;
                outY = y;
            }
        }
    }
    return minEntropy;
}

void WFC::collapse(int x, int y)
{
    auto& cell = grid[y][x];
    if (cell.collapsed || cell.possibleTiles.empty()) return;

    std::vector<float> weights;
    for (int idx : cell.possibleTiles)
        weights.push_back(tileSet[idx].weight);

    std::discrete_distribution<> dist(weights.begin(), weights.end());
    int chosenIdx = cell.possibleTiles[dist(rng)];

    cell.possibleTiles = { chosenIdx };
    cell.collapsed = true;
    generatedMap[y][x] = tileSet[chosenIdx].id;
}

void WFC::propagate(int sx, int sy)
{
    std::stack<std::pair<int, int>> stack;
    stack.emplace(sx, sy);

    while (!stack.empty())
    {
        auto [x, y] = stack.top();
        stack.pop();

        if (!grid[y][x].collapsed) continue;

        int currentTile = grid[y][x].possibleTiles[0];

        const std::array<std::pair<int, int>, 4> dirs = { { {0,-1},{1,0},{0,1},{-1,0} } }; // N E S W

        for (int d = 0; d < 4; ++d)
        {
            int nx = x + dirs[d].first;
            int ny = y + dirs[d].second;

            if (nx < 0 || nx >= (int)grid[0].size() || ny < 0 || ny >= (int)grid.size())
                continue;

            auto& neighbor = grid[ny][nx];
            if (neighbor.collapsed) continue;

            bool changed = false;
            std::vector<int> newPossibles;

            for (int t : neighbor.possibleTiles)
            {
                if (compatible(currentTile, t, d))
                    newPossibles.push_back(t);
            }

            if (newPossibles.size() != neighbor.possibleTiles.size())
            {
                neighbor.possibleTiles = std::move(newPossibles);
                changed = true;
            }

            if (changed)
            {
                if (neighbor.possibleTiles.empty())
                {
                    return;
                }

                if (neighbor.possibleTiles.size() == 1)
                {
                    collapse(nx, ny);
                }
                stack.emplace(nx, ny);
            }
        }
    }
}

bool WFC::isGenerationValid() const
{
    for (const auto& row : grid)
        for (const auto& cell : row)
            if (!cell.collapsed)
                return false;
    return true;
}

bool WFC::generateDungeon(int width, int height, uint32_t seed, int maxAttempts)
{
    if (width > MAX_DUNGEON_SIZE || height > MAX_DUNGEON_SIZE || width < 5 || height < 5)
        return false;

    if (seed != 0) rng.seed(seed);

    lastWidth = width;
    lastHeight = height;

    for (int attempt = 0; attempt < maxAttempts; ++attempt)
    {
        // Reset grid
        grid.assign(height, std::vector<Cell>(width));
        for (auto& row : grid)
            for (auto& cell : row)
            {
                cell.possibleTiles.resize(tileSet.size());
                std::iota(cell.possibleTiles.begin(), cell.possibleTiles.end(), 0);
                cell.collapsed = false;
            }

        memset(generatedMap, 0, sizeof(generatedMap));

        int cx = width / 2;
        int cy = height / 2;

        // Start with floor in center
        collapse(cx, cy);
        propagate(cx, cy);

        // Add a few more starting points for larger dungeons
        if (width > 10 && height > 10)
        {
            collapse(cx + width / 4, cy);
            propagate(cx + width / 4, cy);

            collapse(cx - width / 4, cy + height / 4);
            propagate(cx - width / 4, cy + height / 4);
        }

        // Main collapse loop
        int iterations = 0;
        while (iterations++ < width * height * 2)
        {
            int x, y;
            if (getLowestEntropy(x, y) == INT_MAX) break;

            collapse(x, y);
            propagate(x, y);
        }

        if (isGenerationValid())
        {
            // Post-process: Ensure borders are walls
            for (int x = 0; x < width; ++x)
            {
                generatedMap[0][x] = 1;
                generatedMap[height - 1][x] = 1;
            }
            for (int y = 0; y < height; ++y)
            {
                generatedMap[y][0] = 1;
                generatedMap[y][width - 1] = 1;
            }

            // Add entrance on left side
            generatedMap[height / 2][0] = 7; // Door

            return true;
        }
    }

    return false;
}

void WFC::placeInWorld(int worldX, int worldY)
{
    for (int dy = 0; dy < lastHeight; ++dy)
    {
        for (int dx = 0; dx < lastWidth; ++dx)
        {
            int wx = worldX + dx;
            int wy = worldY + dy;

            if (wx < 0 || wx >= MAP_WIDTH || wy < 0 || wy >= MAP_HEIGHT)
                continue;

            int tile = generatedMap[dy][dx];

            if (tile != 0)
            {
                map[wy][wx] = tile;
                sectorMap[wy][wx] = 3; // Dungeon sector
            }
        }
    }

    // Clear area around entrance
    int entranceY = worldY + lastHeight / 2;
    for (int dx = -2; dx < 0; ++dx)
    {
        int wx = worldX + dx;
        if (wx >= 0 && wx < MAP_WIDTH && entranceY >= 0 && entranceY < MAP_HEIGHT)
        {
            map[entranceY][wx] = 0;      // Clear floor
            map[entranceY - 1][wx] = 0;    // Wider entrance
            map[entranceY + 1][wx] = 0;
            sectorMap[entranceY][wx] = 0; // Back to outdoor sector
        }
    }
}

void WFC::renderMiniMap() 
{
    // TODO: Add this
}