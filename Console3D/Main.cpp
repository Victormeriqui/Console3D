#include <stdio.h>
#include <Windows.h>
#include <math.h>

#include "Screen.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "Color.hpp"

using namespace Render;

void SetSmallestFont(HANDLE* hOut);

int main()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	
	CONSOLE_CURSOR_INFO cursorinfo;
	GetConsoleCursorInfo(hOut, &cursorinfo);
	cursorinfo.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cursorinfo);

	SetConsoleTitle(TEXT("Console 3D"));
	SetSmallestFont(&hOut);

	int width = 1000;
	int height = 700;
	
	Screen screen = Screen(width, height, &hOut);
	SetConsoleScreenBufferInfoEx(hOut, screen.GetConsoleScreenBuffer());

	
	while (1)
	{
		screen.SetPixel(10, 10, BG_YELLOW | FG_RED, 32);
		screen.SetPixel(12, 10, BG_YELLOW | FG_RED, 176);
		screen.SetPixel(14, 10, BG_YELLOW | FG_RED, 177);
		screen.SetPixel(16, 10, BG_YELLOW | FG_RED, 178);
		screen.SetPixel(18, 10, BG_YELLOW | FG_RED, 219);
		
		
		screen.DrawFrame();
		screen.ClearFrame(COL_BACKGROUND);
	}
	//Scene scene = Scene(&screen);
	//Engine engine = Engine(&screen, &scene, &hOut, &hIn);
	
	//engine.Start();
	
	return 0;
}

void SetSmallestFont(HANDLE* hOut)
{
	COORD fontsize = { 12, 16 };
	CONSOLE_FONT_INFOEX font;

	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.dwFontSize = fontsize;
	font.FontFamily = 0;
	font.FontWeight = 0;
	wcscpy_s(font.FaceName, L"Raster Fonts");
	SetCurrentConsoleFontEx(*hOut, FALSE, &font);
}