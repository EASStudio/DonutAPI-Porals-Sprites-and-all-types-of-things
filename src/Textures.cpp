#include "Textures.h"

/*
Textures:

Wall textures:

Cave/Dungeon roof                  | Sky / Roof Texture
Church/Black smith hut/Castle roof | Sky / Roof Texture
House roof                         | Sky / Roof Texture
Bricks                             | 1   | Done
Black Bricks                       | 2   | Done
Stone                              | 3   | Done
Black Stone                        | 4   | Done
Cobble Stone                       | 5   | Done
Wooden Planks                      | 6   | Done
Wooden Beams                       | 7   | Done
Clear Glass                        | 8   | Done
Red Glass                          | 9   | Done
Green Glass                        | 10  | Done
Blue Glass                         | 11  | Done
Banner                             | 12  | Done

Floor Textures:
Grass                              | 13 | Done                          
Gravel                             | 14 | Done
Carpet                             | 15 | Done
Red Carpet                         | 16 | Done
Blue Carpet                        | 17 | Done

Sector/Y coord Textures:
White Slope                        | 18 | Done
Black Slope                        | 19 | Done
Dark Gray Slope                    | 20 | Done
Gray Slope                         | 21 | Done

*/

// Wall textures
void Textures::brickTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            if (y % 4 == 0) { SetTexPixel(tex, x, y, DARK_GRAY); continue; }
            int mortar = ((y / 4) % 2 == 0) ? 7 : 15;
            SetTexPixel(tex, x, y, (x == mortar % 16) ? DARK_GRAY : ((x + y) % 3 == 0 ? RED : DARK_RED));
        }
    }
}

void Textures::blackBrickTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            if (y % 4 == 0) { SetTexPixel(tex, x, y, DARK_GRAY); continue; }
            int mortar = ((y / 4) % 2 == 0) ? 7 : 15;
            SetTexPixel(tex, x, y, (x == mortar % 16) ? DARK_GRAY : ((x + y) % 3 == 0 ? DARK_GRAY : DARK_GRAY));
        }
    }
}

void Textures::stoneTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, GRAY); 
    }
}

void Textures::blackStoneTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, DARK_GRAY);
    }
}

void Textures::cobbleStoneTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            bool h = (y % 5 == 0) || (y % 5 == 4 && x > 8);
            bool v = (x % 6 == 0) || (x % 6 == 5 && y < 8);
            SetTexPixel(tex, x, y, (h || v) ? GRAY : DARK_GRAY);
        }
    }
}

void Textures::woodTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x % 4 == 0) ? YELLOW : BROWN);
    }
}

void Textures::woodenBeamTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (y % 4 == 0) ? YELLOW : BROWN);
    }
}

void Textures::glassTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 3 == 0 ? WHITE : BLACK);  // TODO: Fix the black color | Doesnt render properly; just has an opach color
    }
}

void Textures::rGlassTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 3 == 0 ? WHITE : RED);
    }
}

void Textures::gGlassTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 3 == 0 ? WHITE : GREEN);
    }
}

void Textures::bGlassTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 3 == 0 ? WHITE : BLUE);
    }
}

void Textures::bannerTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y + y) % 4 ? BLUE : RED);
    }
}


// Floor Texture


void Textures::grassTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 4 ? DARK_GREEN : GREEN);
    }
}

void Textures::gravelTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 4 ? DARK_GRAY : GRAY);
    }
}

void Textures::carpetTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 4 ? MAGENTA : DARK_MAGENTA);
    }
}

void Textures::rCarpetTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 4 ? DARK_RED : RED);
    }
}

void Textures::bCarpetTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, (x + y) % 4 ? DARK_BLUE : BLUE);
    }
}

// Sector / Y coord Textures


void Textures::wSlopeTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, WHITE);
    }
}

void Textures::bSlopeTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, BLACK);
    }
}
void Textures::dgSlopeTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, DARK_GRAY);
    }
}

void Textures::gSlopeTile(Texture& tex)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
            SetTexPixel(tex, x, y, DARK_GRAY);
    }
}

// Test for tree and mountain 
void Textures::treeTile(Texture& tex)
{
    const unsigned short G = COLOR::GREEN;
    const unsigned short D = COLOR::DARK_GREEN;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK; // Background / transparent space

    unsigned short treeGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, G, G, G, G, _, _, _, _, _, _ },
        { _, _, _, _, _, G, G, D, D, G, G, _, _, _, _, _ },
        { _, _, _, _, G, G, D, D, D, D, G, G, _, _, _, _ },
        { _, _, _, G, G, D, D, D, D, D, D, G, G, _, _, _ },
        { _, _, G, G, G, D, D, D, D, D, D, G, G, G, _, _ },
        { _, _, G, G, G, G, D, D, D, D, G, G, G, G, _, _ },
        { _, _, G, G, G, G, G, G, G, G, G, G, G, G, _, _ },
        { _, _, _, G, G, G, G, G, G, G, G, G, G, _, _, _ },
        { _, _, _, _, G, G, G, G, G, G, G, G, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, B, B, B, B, B, B, _, _, _, _, _ },
        { _, _, _, _, B, B, B, B, B, B, B, B, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y) 
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(treeTex, x, y, treeGrid[y][x]);
        }
    }
}

void Textures::dgTreeTile(Texture& tex)
{
    const unsigned short G = COLOR::DARK_GREEN;
    const unsigned short D = COLOR::GREEN;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK; // Background / transparent space

    unsigned short dgTreeGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, G, G, G, G, _, _, _, _, _, _ },
        { _, _, _, _, _, G, G, D, D, G, G, _, _, _, _, _ },
        { _, _, _, _, G, G, D, D, D, D, G, G, _, _, _, _ },
        { _, _, _, G, G, D, D, D, D, D, D, G, G, _, _, _ },
        { _, _, G, G, G, D, D, D, D, D, D, G, G, G, _, _ },
        { _, _, G, G, G, G, D, D, D, D, G, G, G, G, _, _ },
        { _, _, G, G, G, G, G, G, G, G, G, G, G, G, _, _ },
        { _, _, _, G, G, G, G, G, G, G, G, G, G, _, _, _ },
        { _, _, _, _, G, G, G, G, G, G, G, G, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, B, B, B, B, B, B, _, _, _, _, _ },
        { _, _, _, _, B, B, B, B, B, B, B, B, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x) 
        {
            SetTexPixel(dgTreeTex, x, y, dgTreeGrid[y][x]);
        }
    }
}

void Textures::sTreeTile(Texture& tex)
{
    const unsigned short G = COLOR::WHITE;
    const unsigned short D = COLOR::DARK_GREEN;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK; // Background / transparent space

    unsigned short sTreeGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, G, G, G, G, _, _, _, _, _, _ },
        { _, _, _, _, _, G, G, G, G, G, G, _, _, _, _, _ },
        { _, _, _, _, G, G, D, D, D, D, G, G, _, _, _, _ },
        { _, _, _, G, G, D, D, D, D, D, D, G, G, _, _, _ },
        { _, _, G, G, D, D, D, D, D, D, D, D, G, G, _, _ },
        { _, _, G, G, D, D, D, D, D, D, D, D, G, G, _, _ },
        { _, _, G, G, D, D, D, D, D, D, D, D, G, G, _, _ },
        { _, _, _, G, D, D, D, D, D, D, D, D, G, _, _, _ },
        { _, _, _, _, D, D, D, D, D, D, D, D, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, B, B, B, B, B, B, _, _, _, _, _ },
        { _, _, _, _, B, B, B, B, B, B, B, B, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y) 
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(sTreeTex, x, y, sTreeGrid[y][x]);
        }
    }
}

void Textures::bushTile(Texture& tex)
{
    const unsigned short G = COLOR::GREEN;
    const unsigned short D = COLOR::DARK_GREEN;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK;

    unsigned short bushGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, G, G, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, G, D, D, G, _, _, _, _, _, _ },
        { _, _, _, _, _, G, G, D, D, G, G, _, _, _, _, _ },
        { _, _, _, _, _, G, D, D, D, D, G, _, _, _, _, _ },
        { _, _, _, _, _, _, G, G, G, G, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, _, _, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(bushTex, x, y, bushGrid[y][x]);
        }
    }
}

void Textures::sBushTile(Texture& tex)
{
    const unsigned short G = COLOR::WHITE;
    const unsigned short D = COLOR::DARK_GREEN;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK;

    unsigned short sBushGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, G, G, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, G, G, G, G, _, _, _, _, _, _ },
        { _, _, _, _, _, G, G, D, D, G, G, _, _, _, _, _ },
        { _, _, _, _, _, G, D, D, D, D, G, _, _, _, _, _ },
        { _, _, _, _, _, _, D, D, D, D, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, _, _, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(sBushTex, x, y, sBushGrid[y][x]);
        }
    }
}

void Textures::rightArmTile(Texture& tex) 
{
    unsigned short skin = YELLOW; 
    unsigned short sleeve = DARK_GRAY;
    unsigned short shadow = DARK_GRAY;

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            if (y >= 10) // hand area
            {
                if (x >= 6 && x <= 13 && y >= 11)
                    SetTexPixel(tex, x, y, skin);
                else
                    SetTexPixel(tex, x, y, 0); // transparent
            }

            else if (y >= 5) // forearm
            {
                if (x >= 5 && x <= 11)
                    SetTexPixel(tex, x, y, skin);
                else if (x >= 4 && x <= 12)
                    SetTexPixel(tex, x, y, sleeve);
                else
                    SetTexPixel(tex, x, y, 0);
            }

            else // upper arm
            {
                if (x >= 6 && x <= 10)
                    SetTexPixel(tex, x, y, skin);
                else
                    SetTexPixel(tex, x, y, 0);
            }
        }
    }
}

void Textures::leftArmTile(Texture& tex)
{
    unsigned short skin = YELLOW;      
    unsigned short sleeve = DARK_GRAY;
    unsigned short shadow = DARK_GRAY;

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            if (y >= 10) // hand area
            {
                if (x >= 6 && x <= 13 && y >= 11)
                    SetTexPixel(tex, x, y, skin);
                else
                    SetTexPixel(tex, x, y, 0); // transparent
            }

            else if (y >= 5) // forearm
            {
                if (x >= 5 && x <= 11)
                    SetTexPixel(tex, x, y, skin);
                else if (x >= 4 && x <= 12)
                    SetTexPixel(tex, x, y, sleeve);
                else
                    SetTexPixel(tex, x, y, 0);
            }

            else // upper arm
            {
                if (x >= 6 && x <= 10)
                    SetTexPixel(tex, x, y, skin);
                else
                    SetTexPixel(tex, x, y, 0);
            }
        }
    }
}

void Textures::horseTile(Texture& tex)
{
    const unsigned short B = COLOR::BROWN;
    const unsigned short G = COLOR::GRAY;
    const unsigned short W = COLOR::WHITE;
    const unsigned short _ = COLOR::BLACK;

    unsigned short horseGrid[16][16] = 
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, B, W, B, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, W, G, B, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, W, B, B, B, W, B, B, B, _, _, _ },
        { _, _, _, _, _, _, W, B, B, B, B, W, B, _, _, _ },
        { _, _, _, _, _, _, W, B, W, W, W, B, W, B, B, _ },
        { _, _, _, _, _, W, W, _, _, _, _, _, W, B, _, B },
        { _, _, _, _, _, B, W, _, _, _, _, _, W, W, _, _ },
        { _, _, _, _, _, W, W, _, _, _, _, _, W, W, _, _ },
        { _, _, _, _, _, B, B, _, _, _, _, _, B, B, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(horseTex, x, y, horseGrid[y][x]);
        }
    }
}

void Textures::salesmanTile(Texture& tex)
{
    const unsigned short A = COLOR::RED;
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short G = COLOR::GRAY;
    const unsigned short R = COLOR::DARK_GRAY;
    const unsigned short D = COLOR::DARK_GREEN;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK;

    unsigned short salesmanGrid[16][16] = 
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, A, A, A, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, Y, Y, A, _, _, _, _, _, _ },
        { _, _, _, _, _, _, Y, D, B, D, Y, _, _, _, _, _ },
        { _, _, _, _, _, _, B, Y, B, Y, B, _, _, _, _, _ },
        { _, _, _, _, _, _, _, Y, Y, Y, _, _, _, _, _, _ },
        { _, _, _, _, _, D, D, D, G, D, D, D, _, _, _, _ },
        { _, _, _, _, D, D, D, D, G, G, D, D, D, _, _, _ },
        { _, _, _, D, D, _, D, G, G, G, D, _, D, D, _, _ },
        { _, _, _, Y, Y, _, D, D, G, D, D, _, Y, Y, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, R, R, _, R, R, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(salesmanTex, x, y, salesmanGrid[y][x]);
        }
    }
}

void Textures::blacksmithTile(Texture& tex)
{
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short G = COLOR::GRAY;
    const unsigned short R = COLOR::DARK_GRAY;
    const unsigned short D = COLOR::DARK_GREEN;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK;

    unsigned short blacksmithGrid[16][16] =
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, R, R, R, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, Y, Y, Y, _, _, _, _, _, _ },
        { _, _, _, _, _, _, Y, B, Y, B, Y, _, _, _, _, _ },
        { _, _, _, _, _, _, Y, Y, B, Y, Y, _, _, _, _, _ },
        { _, _, _, _, _, _, _, R, R, R, _, _, _, _, _, _ },
        { _, _, _, _, _, D, D, D, D, D, D, D, _, _, _, _ },
        { _, _, _, _, D, D, D, D, D, D, D, D, D, _, _, _ },
        { _, Y, B, Y, Y, _, B, B, G, B, B, _, Y, Y, _, _ },
        { _, B, B, B, B, _, D, D, D, D, D, _, Y, Y, _, _ },
        { _, B, B, _, _, _, D, D, _, D, D, _, _, _, _, _ },
        { _, _, _, _, _, _, D, D, _, D, D, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(blacksmithTex, x, y, blacksmithGrid[y][x]);
        }
    }
}

void Textures::butcherTile(Texture& tex)
{
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short G = COLOR::GRAY;
    const unsigned short R = COLOR::DARK_GRAY;
    const unsigned short D = COLOR::DARK_CYAN;
    const unsigned short M = COLOR::DARK_MAGENTA;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK;

    unsigned short butcherGrid[16][16] =
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, Y, Y, Y, _, _, _, _, _, _ },
        { _, _, _, _, _, _, Y, D, Y, D, Y, _, _, _, _, _ },
        { _, _, _, _, _, _, Y, B, B, B, Y, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, Y, B, _, _, _, _, _, _ },
        { _, _, _, _, _, D, M, D, D, D, M, D, _, _, _, _ },
        { _, _, _, _, D, M, M, D, D, D, M, M, D, _, _, _ },
        { _, _, G, Y, Y, _, B, B, G, B, B, _, Y, Y, _, _ },
        { _, G, G, B, B, _, G, G, G, G, G, _, Y, Y, _, _ },
        { _, _, G, _, _, _, G, G, _, G, G, _, _, _, _, _ },
        { _, _, _, _, _, _, G, G, _, G, G, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(butcherTex, x, y, butcherGrid[y][x]);
        }
    }
}

void Textures::maleCitizen(Texture& tex)
{
    const unsigned short A = COLOR::RED;
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short G = COLOR::GRAY;
    const unsigned short R = COLOR::DARK_GRAY;
    const unsigned short D = COLOR::DARK_BLUE;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK;

    unsigned short maleGrid[16][16] =
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, Y, Y, Y, _, _, _, _, _, _ },
        { _, _, _, _, _, _, Y, D, Y, D, Y, _, _, _, _, _ },
        { _, _, _, _, _, _, Y, B, Y, B, Y, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, D, D, D, G, D, D, D, _, _, _, _ },
        { _, _, _, _, D, D, D, D, G, G, D, D, D, _, _, _ },
        { _, _, _, D, D, _, D, G, G, G, D, _, D, D, _, _ },
        { _, _, _, Y, Y, _, D, D, G, D, D, _, Y, Y, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, R, R, _, R, R, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(maleTex, x, y, maleGrid[y][x]);
        }
    }
}

void Textures::femaleCitizen(Texture& tex)
{
    const unsigned short A = COLOR::RED;
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short G = COLOR::GRAY;
    const unsigned short R = COLOR::DARK_GRAY;
    const unsigned short D = COLOR::MAGENTA;
    const unsigned short B = COLOR::BROWN;
    const unsigned short _ = COLOR::BLACK;

    unsigned short femaleGrid[16][16] =
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, B, Y, Y, Y, B, _, _, _, _, _ },
        { _, _, _, _, _, B, Y, D, Y, D, Y, B, _, _, _, _ },
        { _, _, _, _, _, B, Y, Y, Y, Y, Y, B, _, _, _, _ },
        { _, _, _, _, _, B, B, Y, Y, Y, B, B, _, _, _, _ },
        { _, _, _, _, _, D, D, D, G, D, D, D, _, _, _, _ },
        { _, _, _, _, D, D, D, D, G, G, D, D, D, _, _, _ },
        { _, _, _, D, D, _, D, G, G, G, D, _, D, D, _, _ },
        { _, _, _, Y, Y, _, D, D, G, D, D, _, Y, Y, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, _, B, B, _, _, _, _, _ },
        { _, _, _, _, _, _, R, R, _, R, R, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(femaleTex, x, y, femaleGrid[y][x]);
        }
    }
}

void Textures::boyCitizen(Texture& tex)
{

}

void Textures::girlCitizen(Texture& tex)
{

}

void Textures::skeletonTile(Texture& tex)
{
    const unsigned short A = COLOR::RED;
    const unsigned short W = COLOR::WHITE;
    const unsigned short _ = COLOR::BLACK;

    unsigned short skeletonGrid[16][16] = 
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, W, W, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, W, W, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, A, W, A, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, _, W, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, W, W, _, _, _, _, _, _ },
        { _, _, _, _, _, W, W, _, W, _, W, W, _, _, _, _ },
        { _, _, _, _, W, W, W, _, W, _, W, W, W, _, _, _ },
        { _, _, _, W, W, _, W, _, W, _, W, _, W, W, _, _ },
        { _, _, _, W, W, _, W, W, W, W, W, _, W, W, _, _ },
        { _, _, _, _, _, _, W, W, _, W, W, _, _, _, _, _ },
        { _, _, _, _, _, _, W, W, _, W, W, _, _, _, _, _ },
        { _, _, _, _, _, _, W, W, _, W, W, _, _, _, _, _ },
        { _, _, _, _, _, _, W, W, _, W, W, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(skeletonTex, x, y, skeletonGrid[y][x]);
        }
    }
}

void Textures::zombieTile(Texture& tex)
{
    const unsigned short G = COLOR::GREEN;
    const unsigned short R = COLOR::RED;
    const unsigned short W = COLOR::WHITE;
    const unsigned short B = COLOR::BLUE;
    const unsigned short A = COLOR::GRAY;
    const unsigned short _ = COLOR::BLACK;

    unsigned short zombieGrid[16][16] =
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, G, G, G, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, G, G, G, _, _, _, _, _, _ },
        { _, _, _, _, _, _, G, R, W, R, G, _, _, _, _, _ },
        { _, _, _, _, _, _, G, G, W, G, G, _, _, _, _, _ },
        { _, _, _, _, _, _, _, G, G, G, _, _, _, _, _, _ },
        { _, _, _, _, _, G, B, G, G, B, G, G, _, _, _, _ },
        { _, _, _, _, G, B, B, B, G, G, B, W, W, _, _, _ },
        { _, _, _, G, G, _, G, B, B, B, G, _, W, G, _, _ },
        { _, _, _, G, G, _, B, B, G, G, G, _, G, G, _, _ },
        { _, _, _, _, _, _, G, A, _, A, A, _, _, _, _, _ },
        { _, _, _, _, _, _, A, G, _, W, W, _, _, _, _, _ },
        { _, _, _, _, _, _, A, A, _, A, W, _, _, _, _, _ },
        { _, _, _, _, _, _, A, G, _, G, W, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            SetTexPixel(zombieTex, x, y, zombieGrid[y][x]);
        }
    }
}

void Textures::witchTile(Texture& tex)
{
    const unsigned short _ = COLOR::BLACK;
    const unsigned short B = COLOR::DARK_GRAY;
    const unsigned short G = COLOR::GREEN;
    const unsigned short D = COLOR::DARK_GREEN;
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short A = COLOR::BROWN;
    const unsigned short M = COLOR::DARK_MAGENTA;

    unsigned short witchGrid[16][16] =
    {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, B, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, B, B, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, B, G, B, _, _, _, _, _, _ },
        { _, _, _, B, _, _, B, B, B, B, B, _, _, _, _, _ },
        { _, _, B, G, B, _, _, Y, Y, Y, _, _, _, _, _, _ },
        { _, _, _, B, _, _, Y, M, A, M, Y, _, _, _, _, _ },
        { _, _, _, B, _, _, _, Y, A, Y, _, _, _, _, _, _ },
        { _, _, _, B, _, M, M, M, D, M, M, M, _, _, _, _ },
        { _, _, _, B, M, M, M, M, D, M, M, M, M, _, _, _ },
        { _, _, _, B, M, _, M, M, D, M, M, _, M, M, _, _ },
        { _, _, _, B, Y, _, M, M, D, M, M, _, Y, Y, _, _ },
        { _, _, _, B, _, _, M, M, D, M, M, _, _, _, _, _ },
        { _, _, _, B, _, _, M, M, D, M, M, _, _, _, _, _ },
        { _, _, _, _, _, _, M, M, D, M, M, _, _, _, _, _ },
        { _, _, _, _, _, _, M, M, D, M, M, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x)
            SetTexPixel(witchTex, x, y, witchGrid[y][x]);
}

void Textures::bearTile(Texture& tex)
{
    const unsigned short _ = COLOR::BLACK;
    const unsigned short B = COLOR::BROWN;
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short K = COLOR::DARK_GRAY;

    unsigned short bearGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, B, _, B, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, B, B, B, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, K, B, B, B, B, B, B, B, B, _, _ },
        { _, _, _, _, Y, Y, B, B, B, B, B, B, B, B, B, _ },
        { _, _, _, _, K, Y, B, B, B, B, B, B, B, B, B, _ },
        { _, _, _, _, _, B, B, B, B, B, B, B, B, B, B, _ },
        { _, _, _, _, _, B, B, B, B, B, B, B, B, B, _, _ },
        { _, _, _, _, _, B, B, _, _, _, _, _, B, B, _, _ },
        { _, _, _, _, _, B, B, _, _, _, _, _, B, B, _, _ }
    };

    for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x)
            SetTexPixel(bearTex, x, y, bearGrid[y][x]);
}

void Textures::vampireTile(Texture& tex)
{
    const unsigned short _ = COLOR::BLACK;
    const unsigned short K = COLOR::DARK_GRAY;
    const unsigned short R = COLOR::DARK_RED;
    const unsigned short M = COLOR::DARK_MAGENTA;
    const unsigned short W = COLOR::WHITE;

    unsigned short vampireGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, K, K, K, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, W, W, _, _, _, _, _, _ },
        { _, _, _, _, _, _, W, R, W, R, W, _, _, _, _, _ },
        { _, _, _, _, _, _, W, W, W, W, W, _, _, _, _, _ },
        { _, _, _, _, _, K, K, W, W, W, K, K, _, _, _, _ },
        { _, _, _, _, _, _, K, K, K, K, K, _, _, _, _, _ },
        { _, _, _, _, _, _, _, K, K, K, _, _, _, _, _, _ },
        { _, _, _, _, _, K, K, K, K, K, K, K, _, _, _, _ },
        { _, _, _, _, K, K, K, K, K, K, K, K, K, _, _, _ },
        { _, _, _, K, K, M, K, K, K, K, K, M, K, K, _, _ },
        { _, _, _, W, W, M, K, K, K, K, K, M, W, W, _, _ },
        { _, _, _, _, M, M, K, K, _, K, K, M, M, _, _, _ },
        { _, _, _, M, M, M, K, K, _, K, K, M, M, M, _, _ },
        { _, _, _, M, M, M, K, K, _, K, K, M, M, M, _, _ },
        { _, _, _, _, _, _, K, K, _, K, K, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x)
            SetTexPixel(vampireTex, x, y, vampireGrid[y][x]);
}

void Textures::lichTile(Texture& tex)
{
    const unsigned short _ = COLOR::BLACK;
    const unsigned short R = COLOR::RED;
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short W = COLOR::WHITE;

    unsigned short lichGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, R, W, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, W, W, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, R, W, R, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, _, W, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, W, W, W, _, _, _, _, _, _ },
        { _, _, _, _, _, R, R, R, R, R, R, R, _, _, _, _ },
        { _, _, _, _, R, R, R, Y, R, Y, R, R, R, _, _, _ },
        { _, _, _, R, R, _, R, Y, R, Y, R, _, R, R, _, _ },
        { _, _, _, W, W, _, R, Y, R, Y, R, _, W, W, _, _ },
        { _, _, _, _, _, R, R, R, Y, R, R, R, _, _, _, _ },
        { _, _, _, _, _, Y, R, R, R, R, R, Y, _, _, _, _ },
        { _, _, _, _, _, Y, R, R, R, R, R, Y, _, _, _, _ },
        { _, _, _, _, _, R, R, R, R, R, R, R, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x)
            SetTexPixel(lichTex, x, y, lichGrid[y][x]);
}

void Textures::mageTile(Texture& tex)
{
    const unsigned short _ = COLOR::BLACK;
    const unsigned short B = COLOR::BLUE;
    const unsigned short D = COLOR::DARK_BLUE;
    const unsigned short K = COLOR::DARK_GRAY;
    const unsigned short Y = COLOR::YELLOW;
    const unsigned short A = COLOR::BROWN;
    const unsigned short W = COLOR::WHITE;

    unsigned short mageGrid[16][16] = {
        { _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _ },
        { _, _, _, _, _, _, _, _, B, _, _, _, _, _, _, _ },
        { _, _, _, Y, _, _, _, B, B, B, _, _, _, _, _, _ },
        { _, _, Y, Y, Y, _, _, B, K, B, _, _, _, _, _, _ },
        { _, _, _, Y, _, _, B, W, K, W, B, _, _, _, _, _ },
        { _, _, _, A, _, _, B, K, K, K, B, _, _, _, _, _ },
        { _, _, _, A, D, _, B, K, K, K, B, _, D, _, _, _ },
        { _, _, _, A, B, B, B, B, B, B, B, B, B, _, _, _ },
        { _, _, _, A, _, B, B, B, B, D, B, B, _, _, _, _ },
        { _, _, _, A, B, B, B, B, B, B, B, B, B, _, _, _ },
        { _, _, _, A, B, _, B, D, K, B, B, _, B, B, _, _ },
        { _, _, _, A, Y, _, B, B, K, D, B, _, Y, Y, _, _ },
        { _, _, _, A, _, _, D, B, K, B, B, _, _, _, _, _ },
        { _, _, _, A, _, _, B, D, K, B, D, _, _, _, _, _ },
        { _, _, _, A, _, _, B, D, K, B, D, _, _, _, _, _ },
        { _, _, _, _, _, _, B, B, K, D, B, _, _, _, _, _ }
    };

    for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x)
            SetTexPixel(mageTex, x, y, mageGrid[y][x]);
}
void Textures::createTextures()
{
    brickTile(brickTex);
    blackBrickTile(blackBrickTex);
    stoneTile(stoneTex);
    blackStoneTile(blackStoneTex);
    cobbleStoneTile(cobbleTex);
    woodTile(woodTex);
    woodenBeamTile(beamTex);
    glassTile(glassTex);
    rGlassTile(rGlassTex);
    gGlassTile(gGlassTex);
    bGlassTile(bGlassTex);
    bannerTile(bannerTex);
    grassTile(grassTex);
    gravelTile(gravelTex);
    carpetTile(carpetTex);
    rCarpetTile(rCarpetTex);
    bCarpetTile(bCarpetTex);
    wSlopeTile(wSlopeTex);
    bSlopeTile(bSlopeTex);
    dgSlopeTile(dgSlopeTex);
    gSlopeTile(gSlopeTex);

    treeTile(treeTex);
    dgTreeTile(dgTreeTex);
    sTreeTile(sTreeTex);
    bushTile(bushTex);
    sBushTile(sBushTex);

    rightArmTile(rightArmTex);
    leftArmTile(leftArmTex);

    horseTile(horseTex);
    salesmanTile(salesmanTex);

    skeletonTile(skeletonTex);
    zombieTile(zombieTex);
    witchTile(witchTex);
    bearTile(bearTex);
    vampireTile(vampireTex);
    lichTile(lichTex);
    mageTile(mageTex);

    texSet.tex[0] = {};       // 0: no texture / sky
    texSet.tex[1] = brickTex;
    texSet.tex[2] = blackBrickTex;
    texSet.tex[3] = stoneTex;
    texSet.tex[4] = blackStoneTex;
    texSet.tex[5] = cobbleTex;
    texSet.tex[6] = woodTex;
    texSet.tex[7] = beamTex;
    texSet.tex[8] = glassTex;
    texSet.tex[9] = rGlassTex;
    texSet.tex[10] = gGlassTex;
    texSet.tex[11] = bGlassTex;
    texSet.tex[12] = bannerTex;
    texSet.tex[13] = grassTex;
    texSet.tex[14] = gravelTex;
    texSet.tex[15] = carpetTex;
    texSet.tex[16] = rCarpetTex;
    texSet.tex[17] = bCarpetTex;
    texSet.tex[18] = wSlopeTex;
    texSet.tex[19] = bSlopeTex;
    texSet.tex[20] = dgSlopeTex;
    texSet.tex[21] = gSlopeTex;

    texSet.tex[22] = treeTex;        
    texSet.tex[23] = dgTreeTex;
    texSet.tex[24] = sTreeTex;
    texSet.tex[25] = bushTex;
    texSet.tex[26] = sBushTex;

    texSet.tex[27] = rightArmTex;
    texSet.tex[28] = leftArmTex;

    texSet.tex[29] = horseTex;

    texSet.tex[30] = salesmanTex;
    texSet.tex[31] = blacksmithTex;
    texSet.tex[32] = butcherTex;
    texSet.tex[33] = maleTex;
    texSet.tex[34] = femaleTex;
    texSet.tex[35] = boyTex;
    texSet.tex[36] = girlTex;

    texSet.tex[37] = skeletonTex;
    texSet.tex[38] = zombieTex;
    texSet.tex[39] = witchTex;
    texSet.tex[40] = bearTex;
    texSet.tex[41] = vampireTex;
    texSet.tex[42] = lichTex;
    texSet.tex[43] = mageTex;

}

void Textures::destroyTextures() 
{
    DestroyTexture(brickTex);
    DestroyTexture(blackBrickTex);
    DestroyTexture(stoneTex);
    DestroyTexture(blackStoneTex);
    DestroyTexture(cobbleTex);
    DestroyTexture(woodTex);
    DestroyTexture(beamTex);
    DestroyTexture(glassTex);
    DestroyTexture(rGlassTex);
    DestroyTexture(gGlassTex);
    DestroyTexture(bGlassTex);
    DestroyTexture(bannerTex);
    DestroyTexture(grassTex);
    DestroyTexture(gravelTex);
    DestroyTexture(carpetTex);
    DestroyTexture(rCarpetTex);
    DestroyTexture(bCarpetTex);
    DestroyTexture(wSlopeTex);
    DestroyTexture(bSlopeTex);
    DestroyTexture(dgSlopeTex);
    DestroyTexture(gSlopeTex);

    DestroyTexture(treeTex);
    DestroyTexture(dgTreeTex);
    DestroyTexture(sTreeTex);
    DestroyTexture(bushTex);
    DestroyTexture(sBushTex);

    DestroyTexture(rightArmTex);
    DestroyTexture(leftArmTex);

    DestroyTexture(horseTex);
    DestroyTexture(salesmanTex);

    DestroyTexture(skeletonTex);
    DestroyTexture(zombieTex);
    DestroyTexture(witchTex);
    DestroyTexture(bearTex);
    DestroyTexture(vampireTex);
    DestroyTexture(lichTex);
    DestroyTexture(mageTex);
}