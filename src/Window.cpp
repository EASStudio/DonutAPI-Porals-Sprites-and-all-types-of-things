#include "DonutAPI.h"

#include "Window.h"

Window::Window(int width, int height, int font_W, int font_H, int fps) 
{
	InitWindow(width, height, font_W, font_H);
	ShowConsoleCursor(false);
	SetWindowName(L"Quest");
	SetFPS(fps);
}

void Window::CloseWindow()
{
	texture.destroyTextures();
	music.stopMusic();
	DestroyWindow();
}