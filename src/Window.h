#pragma once

#include "Textures.h"
#include "Music.h"

class Window 
{
public:
	Window(int width, int height, int font_W, int font_H, int fps);
	void CloseWindow();

private:
	Textures texture;
	MusicSystem music;
};