#include "DonutMath.h"

#include "Raycaster.h"
#include "PerlinNoise.h"

using namespace Screen;

// Permutation table for Perlin noise
static const int perm[512] = 
{
    39, 137, 114, 400, 241, 471, 253, 114, 233, 145, 173, 153, 278, 416, 168, 510, 331, 122, 366, 353, 69, 194, 206, 
    87, 61, 368, 175, 454, 64, 387, 41, 232, 171, 216, 62, 143, 368, 283, 185, 166, 124, 219, 72, 479, 251, 492, 425, 
    319, 457, 1, 141, 9, 470, 304, 140, 12, 77, 106, 431, 122, 265, 441, 256, 414, 52, 436, 428, 97, 394, 248, 500, 176,
    484, 22, 214, 1, 186, 147, 465, 286, 184, 144, 291, 12, 509, 507, 230, 428, 453, 272, 498, 308, 306, 249, 472, 330, 
    169, 79, 368, 476, 313, 345, 28, 345, 405, 410, 51, 236, 226, 112, 73, 273, 358, 512, 449, 463, 353, 115, 199, 501,
    317, 169, 315, 29, 498, 436, 506, 450, 299, 409, 260, 398, 45, 425, 52, 211, 510, 287, 212, 327, 25, 486, 353, 461, 
    169, 390, 81, 495, 126, 9, 29, 17, 447, 510, 38, 44, 288, 336, 465, 478, 247, 467, 243, 304, 401, 49, 331, 207, 336,
    63, 48, 259, 126, 212, 131, 108, 184, 276, 247, 511, 265, 177, 220, 228, 47, 360, 195, 181, 144, 108, 245, 73, 405,
    1, 325, 45, 10, 205, 494, 151, 68, 458, 13, 254, 459, 211, 93, 296, 125, 66, 2, 466, 216, 120, 129, 233, 4, 252, 404, 
    57, 137, 239, 145, 262, 234, 240, 418, 6, 449, 274, 325, 156, 155, 211, 108, 276, 180, 176, 115, 490, 41, 3, 240, 284,
    278, 131, 209, 81, 206, 151, 214, 476, 195, 57, 161, 65, 192, 303, 45, 72, 387, 68, 6, 40, 129, 59, 221, 154, 271, 393,
    417, 116, 202, 479, 38, 10, 97, 250, 8, 342, 394, 5, 349, 509, 321, 393, 131, 374, 64, 311, 203, 324, 266, 74, 291, 214, 
    360, 40, 374, 438, 370, 423, 231, 353, 390, 405, 119, 495, 510, 244, 210, 151, 485, 347, 486, 60, 442, 63, 266, 293, 261, 
    279, 407, 250, 165, 291, 291, 99, 171, 77, 140, 467, 507, 369, 275, 271, 143, 12, 139, 89, 31, 277, 224, 228, 334, 462, 
    97, 354, 175, 352, 396, 197, 287, 154, 69, 346, 198, 290, 268, 327, 48, 116, 418, 298, 423, 382, 132, 202, 301, 206, 369,
    27, 50, 476, 261, 272, 306, 435, 477, 61, 411, 509, 247, 241, 351, 84, 408, 258, 428, 482, 303, 459, 389, 364, 120, 80, 161,
    189, 70, 253, 430, 181, 222, 148, 102, 231, 361, 36, 216, 181, 214, 46, 125, 240, 6, 412, 32, 213, 261, 48, 346, 147, 290,
    500, 415, 442, 355, 154, 320, 263, 70, 110, 492, 421, 261, 15, 499, 512, 80, 48, 397, 243, 399, 196, 355, 70, 437, 249, 97,
    330, 199, 168, 190, 117, 218, 376, 268, 217, 314, 322, 217, 416, 131, 450, 501, 190, 495, 123, 369, 24, 154, 491, 463, 404,
    252, 53, 212, 448, 201, 232, 48, 260, 389, 460, 116, 286, 89, 344, 44, 50, 137, 179, 111, 506, 58, 509, 46, 470, 135, 215, 
    505, 138, 94, 78, 329, 328, 37, 68, 449, 283, 185, 236
};

static float fbm(float x, float y, int octaves = 6, float lacunarity = 2.0f, float gain = 0.5f)
{
    float sum = 0.0f;
    float amp = 1.0f;
    float freq = 1.0f;
    float maxAmp = 0.0f;

    for (int i = 0; i < octaves; ++i)
    {
        sum += Perlin(x * freq, y * freq, perm) * amp;
        maxAmp += amp;
        amp *= gain;
        freq *= lacunarity;
    }
    return sum / maxAmp;
}

static float ridgedNoise(float x, float y, int octaves = 5)
{
    float sum = 0.0f;
    float amp = 1.0f;
    float freq = 1.0f;

    for (int i = 0; i < octaves; ++i)
    {
        float n = Perlin(x * freq, y * freq, perm);
        n = 1.0f - fabsf(n);        // Ridge
        n = n * n; 
        // Sharpen
        sum += n * amp;
        amp *= 0.5f;
        freq *= 2.0f;
    }
    return sum * 2.0f - 1.0f;
}

PerlinNoise::PerlinNoise() 
{

}

void PerlinNoise::initWorld()
{
    std::mt19937 rng(12345);
    mobSpawns.clear();

    // Clear maps
    memset(map, 0, sizeof(map));
    memset(sectorMap, 0, sizeof(sectorMap));

    // 1. Generate base terrain with Perlin noise
    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            float nx = x * 0.022f;
            float ny = y * 0.022f;

            float elevation = fbm(nx, ny, 7, 2.15f, 0.48f);
            float ridge = ridgedNoise(nx * 1.35f, ny * 1.35f, 4);
            float height = elevation * 0.72f + ridge * 0.28f;
            height = Clamp(height, -1.0f, 1.0f);

            float tempNoise = Perlin(nx * 0.75f, ny * 0.75f, perm);
            float temperature = (tempNoise * 0.6f + (1.0f - fabsf((y - MAP_HEIGHT * 0.5f) / (MAP_HEIGHT * 0.5f)))) * 0.7f;
            float moisture = fbm(nx * 1.55f, ny * 1.55f, 5, 2.0f, 0.52f);

            int tile = 0;
            int sector = 0;

            // Trees in high elevation
            if (height > 0.72f)
                tile = (temperature > 0.5f) ? 22 : 23;

            // Stone/gravel patches
            if (moisture > 0.3f && temperature > 0.25f)
                if (Perlin(nx * 5.0f, ny * 5.0f, perm) < 0.65f)
                    tile = (temperature > 0.5f) ? 5 : 3;

            // Sparse decorations (wood, bushes)
            if ((x % 53 == 0 || y % 47 == 0) && height < 0.35f && rng() % 18 == 0)
                tile = (rng() % 3 == 0) ? 25 : 6;

            map[y][x] = tile;
            sectorMap[y][x] = sector;
        }
    }

    placeDungeons();
    placeMobsInTerrain();
}

void PerlinNoise::placeDungeons()
{
    struct DungeonDef
    {
        int worldX, worldY, width, height, seed;
    };

    std::vector<DungeonDef> dungeons = 
    {
        { 25, 25, 15, 12, 42 },   // Small cave
        { 60, 40, 20, 18, 1337 }, // Medium dungeon
        { 100, 80, 25, 22, 999 }, // Large dungeon
    };

    for (const auto& d : dungeons)
    {
        // Make sure dungeon fits in map
        if (d.worldX + d.width >= MAP_WIDTH || d.worldY + d.height >= MAP_HEIGHT)
            continue;

        // Generate the dungeon
        if (wfc.generateDungeon(d.width, d.height, d.seed, 30))
        {
            // Place it in the world
            wfc.placeInWorld(d.worldX, d.worldY);
            placeMobsInDungeon(d.worldX, d.worldY, d.width, d.height);
        }

        else
        {
            // Fallback: simple box room if WFC fails
            for (int y = d.worldY; y < d.worldY + d.height; ++y)
            {
                for (int x = d.worldX; x < d.worldX + d.width; ++x)
                {
                    if (x == d.worldX || x == d.worldX + d.width - 1 ||
                        y == d.worldY || y == d.worldY + d.height - 1)
                    {
                        map[y][x] = 1; // Wall
                    }

                    else
                    {
                        map[y][x] = 14; // Gravel floor
                        sectorMap[y][x] = 3;
                    }
                }
            }

            // Add door
            map[d.worldY + d.height / 2][d.worldX] = 7;
            placeMobsInDungeon(d.worldX, d.worldY, d.width, d.height);
        }
    }
}

void PerlinNoise::placeMobsInDungeon(int worldX, int worldY, int dungeonW, int dungeonH)
{
    std::mt19937 rng(worldX * 1000 + worldY);

    int mobCount = (dungeonW * dungeonH) / 20; 
    mobCount = std::min(mobCount, MAX_MOBS_PER_DUNGEON); 

    for (int i = 0; i < mobCount; ++i)
    {
        // Try random positions inside dungeon (with margin from walls)
        for (int attempt = 0; attempt < 20; ++attempt)
        {
            int mx = worldX + 2 + rng() % (dungeonW - 4);
            int my = worldY + 2 + rng() % (dungeonH - 4);

            // Check if it's a floor tile
            if (map[my][mx] != 0 && map[my][mx] != 14) continue;

            // Convert to world coordinates (center of tile)
            float spawnX = mx + 0.5f;
            float spawnY = my + 0.5f;

            int mobType;
            int roll = rng() % 100;

            if (roll < 25)       mobType = 37; // Skeleton    25%
            else if (roll < 43)  mobType = 38; // Zombie      18%
            else if (roll < 55)  mobType = 39; // Witch       12%
            else if (roll < 65)  mobType = 41; // Vampire     10%
            else if (roll < 70)  mobType = 42; // Lich         5% (rare, tougher)
            else if (roll < 78)  mobType = 43; // Mage         8%
            else if (roll < 90)  mobType = 30; // Salesmen    12% (prisoner/merchant)
            else                 mobType = 29; // Horse       10% (rare, stabled)

            mobSpawns.push_back({ spawnX, spawnY, mobType });
            break;
        }
    }
}

void PerlinNoise::placeMobsInTerrain()
{
    std::mt19937 rng(54321);

    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            int tile = map[y][x];

            // Skip walls, floors, and existing decorations
            if (tile == 0 || tile == 1 || tile == 5 || tile == 6 ||
                tile == 7 || tile == 14 || tile >= 29)
                continue;

            // Random chance to spawn mob near trees/bushes
            if (rng() % 100 < 2) //  < 2 2% chance
            {
                float worldX = x + 0.5f;
                float worldY = y + 0.5f;

                int mobType = 29; 
                int roll = rng() % 100;

                if (tile == 22 || tile == 23 || tile == 24) // Tree tiles - forest habitat
                {
                    mobType = (roll < 55) ? 40 : 29; // Bear common in forest, horse otherwise
                }

                else if (tile == 25 || tile == 26) // Bush tiles
                {
                    if (roll < 45)       mobType = 29; // Horse       45%
                    else if (roll < 85)  mobType = 30; // Villager    40%
                    else                 mobType = 38; // Zombie      15% - rare wandering surface threat
                }

                mobSpawns.push_back({ worldX, worldY, mobType });
            }
        }
    }
}

void PerlinNoise::spawnMobsFromMap(Mob& mobSystem, Coordinator& coord)
{
    for (const auto& spawn : mobSpawns)
    {
        mobSystem.createMob(coord, spawn.x, spawn.y, spawn.textureIndex);
    }
}