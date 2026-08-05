#pragma once

#include "Raycaster.h"

class MapEditor 
{
public:
	enum EditorState
	{
		TILE,
		SECTOR,
		SLOPE,
		WALL_HEIGHT,
		PORTAL       
	};

	struct Editor
	{
		Entity entity;

		int postionX = 0; 
		int postionY = 0; 
		float sensitivity = 0.1f;

		int selectedTile = 1;
		int selectedSector = 1;
		float zoom = 1.0f;       // zoom multiplier
		int viewOffsetX = 0;     // scroll offset in cells
		int viewOffsetY = 0;

		bool enabled = false;
		bool editCeil = false; // Toggle floor/ceil editing
		EditorState state = TILE;
	};
	Editor editor;

public:
	void drawEditor();
	void drawSlopeDir(int screenX, int screenY, int sec, int cellW, int cellH);
	void editorInput();
	void saveMap();
	void loadMap();

private:
	bool editorActive = false;
};