#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <stdlib.h>
#include <thread>

#include "Screen.hpp"
#include "Scene.hpp"
#include "Color.hpp"
#include "Engine.hpp"

using namespace Render;

void SetSmallestFont(HANDLE* hOut, uint8_t fontw, uint8_t fonth, const wchar_t* fontstr);
void SetNormalFont(HANDLE* hOut);

int main()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorinfo;
	GetConsoleCursorInfo(hOut, &cursorinfo);
	cursorinfo.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cursorinfo);

	SetConsoleTitle(TEXT("Console 3D"));


	int width = 800;
	int height = 600;
	uint8_t fontw = 4;
	uint8_t fonth = 6;
	//const COLORREF* palette = PALETTE_DEFAULT;
	const COLORREF* palette = PALETTE_GREYSCALE_2;
	const wchar_t* font = L"Raster Fonts";
	
	/*
	177 CHAR
	4X6 RASTER FONTS
	PERFECT GRID
	*/
	
	SetSmallestFont(&hOut, fontw, fonth, font);

	Screen screen = Screen(width, height, fontw, fonth, palette, &hOut);
	SetConsoleScreenBufferInfoEx(hOut, screen.GetConsoleScreenBuffer());

	Scene scene = Scene(&screen);
	Engine engine = Engine(&screen, &scene, &hOut);

	engine.Start();

	SetNormalFont(&hOut);

	return 0;
}

void SetSmallestFont(HANDLE* hOut, uint8_t fontw, uint8_t fonth, const wchar_t* fontstr)
{
	COORD fontsize = { fontw, fonth };
	
	CONSOLE_FONT_INFOEX font;

	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.dwFontSize = fontsize;
	font.FontFamily = 0;
	font.FontWeight = 0;
	wcscpy_s(font.FaceName, fontstr);
	SetCurrentConsoleFontEx(*hOut, FALSE, &font);
}

/*
	screenbuffer size = 
	80x
	300y

	window size =
	80x
	25y

	font =
	consolas
	size 24
	11x
	24y
*/
void SetNormalFont(HANDLE* hOut)
{
	COORD fontsize = { 11, 24 };
	CONSOLE_FONT_INFOEX font;

	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font.dwFontSize = fontsize;
	font.FontFamily = 0;
	font.FontWeight = 0;
	wcscpy_s(font.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(*hOut, FALSE, &font);
}