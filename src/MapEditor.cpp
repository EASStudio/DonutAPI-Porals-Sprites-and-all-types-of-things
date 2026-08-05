#include "MapEditor.h"
#include "Globals.h"

#include <fstream>

using namespace Screen;

void MapEditor::drawSlopeDir(int screenX, int screenY, int sec, int cellW, int cellH)
{
    if (sec <= 0) return;

    float sx = editor.editCeil ? sectors[sec].ceilSlopeX : sectors[sec].floorSlopeX;
    float sy = editor.editCeil ? sectors[sec].ceilSlopeY : sectors[sec].floorSlopeY;

    if (abs(sx) < 0.01f && abs(sy) < 0.01f) return;

    int cx = screenX + cellW / 2;
    int cy = screenY + cellH / 2;

    DrawLine(cx, cy, cx + (int)(sx * 20), cy + (int)(sy * 20), PIXEL_SOLID, editor.editCeil ? RED : CYAN);
}

void MapEditor::drawEditor()
{
    const int BASE_CELL = 4;
    int cellW = std::max(2, (int)(BASE_CELL * editor.zoom));
    int cellH = cellW;  // Square cells look better in the editor

    // How many cells fit on screen at current zoom
    int visX = Width / cellW + 1;
    int visY = Height / cellH + 1;

    // Clamp scroll so we don't go past the map
    editor.viewOffsetX = std::clamp(editor.viewOffsetX, 0, std::max(0, MAP_WIDTH - visX));
    editor.viewOffsetY = std::clamp(editor.viewOffsetY, 0, std::max(0, MAP_HEIGHT - visY));

    for (int y = editor.viewOffsetY; y < MAP_HEIGHT && y < editor.viewOffsetY + visY; y++)
    {
        for (int x = editor.viewOffsetX; x < MAP_WIDTH && x < editor.viewOffsetX + visX; x++)
        {
            int sx = (x - editor.viewOffsetX) * cellW;
            int sy = (y - editor.viewOffsetY) * cellH;

            int tile = map[y][x];
            int sec = sectorMap[y][x];
            unsigned short color = DARK_GRAY;

            if (tile > 0) color = (tile % 2 == 0) ? RED : DARK_RED;
            if (sec > 0) color += (sec * 2);

            DrawRectangle(sx, sy, cellW - 1, PIXEL_SOLID, color);

            if (sec > 0) drawSlopeDir(sx, sy, sec, cellW, cellH);
        }
    }

    // Cursor — only draw if on screen
    bool cursorVisible =
        editor.postionX >= editor.viewOffsetX && editor.postionX < editor.viewOffsetX + visX &&
        editor.postionY >= editor.viewOffsetY && editor.postionY < editor.viewOffsetY + visY;

    if (cursorVisible)
    {
        int sx = (editor.postionX - editor.viewOffsetX) * cellW;
        int sy = (editor.postionY - editor.viewOffsetY) * cellH;
        DrawRectangle(sx, sy, cellW, PIXEL_SOLID, WHITE);
    }

    // UI
    std::wstring modeNames[] = { L"TILE", L"SECTOR", L"SLOPE", L"HEIGHT", L"PORTAL" };
    DrawString(2, Height / 2 - 2, L"MODE: " + modeNames[editor.state], CYAN);
    DrawString(16, Height / 2 - 2, L"EDITING: " + std::wstring(editor.editCeil ? L"CEILING" : L"FLOOR"), editor.editCeil ? RED : GREEN);
    DrawString(35, Height / 2 - 2, L"Tile: " + std::to_wstring(editor.selectedTile), WHITE);
    DrawString(49, Height / 2 - 2, L"Sec: " + std::to_wstring(editor.selectedSector), WHITE);
    DrawString(63, Height / 2 - 2, L"Zoom: " + std::to_wstring((int)(editor.zoom * 100)) + L"%", WHITE);
    DrawString(77, Height / 2 - 2, L"View: (" + std::to_wstring(editor.viewOffsetX) + L"," + std::to_wstring(editor.viewOffsetY) + L")", GRAY);

    int curSec = sectorMap[editor.postionY][editor.postionX];
    if (curSec > 0)
    {
        DrawString(2, 65, L"H: " + std::to_wstring(editor.editCeil ? sectors[curSec].ceilH : sectors[curSec].floorH), YELLOW);
        DrawString(2, 75, L"SX: " + std::to_wstring(editor.editCeil ? sectors[curSec].ceilSlopeX : sectors[curSec].floorSlopeX), YELLOW);
        DrawString(2, 85, L"SY: " + std::to_wstring(editor.editCeil ? sectors[curSec].ceilSlopeY : sectors[curSec].floorSlopeY), YELLOW);
    }

    DrawString(2, Height / 2, L"[WASD] Move  [+/-] Zoom  [TAB] Game  [P] Pick current tile  [F] Ceil/Floor  [E] Tile cycle  [Q] Sector  [Space] Apply  [V] Save", GRAY);
}

void MapEditor::editorInput()
{
    // Zoom
    if (GetKey(KEY_EQUAL).k_Pressed) editor.zoom = std::clamp(editor.zoom + 0.25f, 0.5f, 4.0f);
    if (GetKey(KEY_MINUS).k_Pressed)  editor.zoom = std::clamp(editor.zoom - 0.25f, 0.5f, 4.0f);

    if (GetKey(KEY_W).k_Pressed || GetKey(KEY_W).k_Held) editor.postionY--;
    if (GetKey(KEY_S).k_Pressed || GetKey(KEY_S).k_Held) editor.postionY++;
    if (GetKey(KEY_A).k_Pressed || GetKey(KEY_A).k_Held) editor.postionX--;
    if (GetKey(KEY_D).k_Pressed || GetKey(KEY_D).k_Held) editor.postionX++;

    editor.postionX = std::clamp(editor.postionX, 0, MAP_WIDTH - 1);
    editor.postionY = std::clamp(editor.postionY, 0, MAP_HEIGHT - 1);

    // Auto-scroll: keep cursor inside visible area 
    const int BASE_CELL = 8;
    int cellW = std::max(2, (int)(BASE_CELL * editor.zoom));
    int visX = Width / cellW;
    int visY = Height / cellW;

    if (editor.postionX < editor.viewOffsetX)              editor.viewOffsetX = editor.postionX;
    if (editor.postionX >= editor.viewOffsetX + visX - 1)  editor.viewOffsetX = editor.postionX - visX + 2;
    if (editor.postionY < editor.viewOffsetY)              editor.viewOffsetY = editor.postionY;
    if (editor.postionY >= editor.viewOffsetY + visY - 1)  editor.viewOffsetY = editor.postionY - visY + 2;
    // Title under cursor
    if (GetKey(KEY_P).k_Pressed)
    {
        editor.selectedTile = map[editor.postionY][editor.postionX];
        int s = sectorMap[editor.postionY][editor.postionX];
        if (s > 0) editor.selectedSector = s;
    }

    // Quick Save
    if (GetKey(KEY_V).k_Pressed) saveMap();

    // State Selection
    if (GetKey(KEY_1).k_Pressed) editor.state = TILE;
    if (GetKey(KEY_2).k_Pressed) editor.state = SECTOR;
    if (GetKey(KEY_3).k_Pressed) editor.state = SLOPE;
    if (GetKey(KEY_4).k_Pressed) editor.state = WALL_HEIGHT;
    if (GetKey(KEY_5).k_Pressed) editor.state = PORTAL;

    if (GetKey(KEY_F).k_Pressed) editor.editCeil = !editor.editCeil;

    // Cycle selection
    if (GetKey(KEY_E).k_Pressed) editor.selectedTile = (editor.selectedTile + 1) % MAX_TEX;
    if (GetKey(KEY_Q).k_Pressed) editor.selectedSector = (editor.selectedSector + 1) % SECTORS;

    int sec = sectorMap[editor.postionY][editor.postionX];
    float delta = 0.05f;

    // Apply / Adjust (Space for +, X for -)
    bool up = GetKey(KEY_SPACE).k_Held || GetKey(KEY_SPACE).k_Pressed;
    bool down = GetKey(KEY_X).k_Held || GetKey(KEY_X).k_Pressed;

    if (up || down)
    {
        float mod = up ? delta : -delta;

        switch (editor.state)
        {
        case TILE:
            if (up) map[editor.postionY][editor.postionX] = editor.selectedTile;
            if (down) map[editor.postionY][editor.postionX] = 0;
            break;

        case SECTOR:
            if (up) sectorMap[editor.postionY][editor.postionX] = editor.selectedSector;
            if (down) sectorMap[editor.postionY][editor.postionX] = 0;
            break;

        case WALL_HEIGHT:
            if (sec > 0)
            {
                if (editor.editCeil) sectors[sec].ceilH += mod;
                else sectors[sec].floorH += mod;
            }
            break;

        case SLOPE:
            if (sec > 0)
            {
                // Use Arrow Keys for Slope Direction while holding Space/X
                if (GetKey(KEY_LEFT).k_Held || GetKey(KEY_RIGHT).k_Held) {
                    float sMod = GetKey(KEY_RIGHT).k_Held ? delta : -delta;
                    if (editor.editCeil) sectors[sec].ceilSlopeX += sMod;
                    else sectors[sec].floorSlopeX += sMod;
                }
                if (GetKey(KEY_UP).k_Held || GetKey(KEY_DOWN).k_Held) {
                    float sMod = GetKey(KEY_UP).k_Held ? -delta : delta;
                    if (editor.editCeil) sectors[sec].ceilSlopeY += sMod;
                    else sectors[sec].floorSlopeY += sMod;
                }
            }
            break;

        case PORTAL:
            break;
        }
    }

    if (GetKey(KEY_TAB).k_Pressed) editor.enabled = false;
}

void MapEditor::saveMap()
{
    std::ofstream f("map.txt");

    f << "Tiles\n";
    // Tiles
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
            f << map[y][x] << " ";
        f << "\n";
    }

    f << "Sectors per tile\n";

    // Sectors per tile
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
            f << sectorMap[y][x] << " ";
        f << "\n";
    }

    f << "Sector definitions\n";

    // Sector definitions
    for (int i = 0; i < SECTORS; i++)
    {
        auto& s = sectors[i];
        f << s.floorH << " " << s.ceilH << " "
            << s.floorSlopeX << " " << s.floorSlopeY << " "
            << s.ceilSlopeX << " " << s.ceilSlopeY << "\n";
    }
}

void MapEditor::loadMap()
{
    std::ifstream f("map.txt");
    if (!f.is_open()) return;

    std::string line;
    // Skip header "Tiles"
    std::getline(f, line);
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            f >> map[y][x];

    // Skip header "Sectors per tile"
    std::getline(f, line); std::getline(f, line);
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            f >> sectorMap[y][x];

    // Skip header "Sector definitions"
    std::getline(f, line); std::getline(f, line);
    for (int i = 0; i < SECTORS; i++) {
        f >> sectors[i].floorH >> sectors[i].ceilH
            >> sectors[i].floorSlopeX >> sectors[i].floorSlopeY
            >> sectors[i].ceilSlopeX >> sectors[i].ceilSlopeY;
    }
    f.close();
}