#include <stdint.h>
#include <math.h>

#include "Screen.hpp"
#include "Color.hpp"
#include "Util.hpp"

using namespace Base;

namespace Render
{

	Screen::Screen(uint16_t width, uint16_t height, HANDLE* hout) :
		width(width), height(height), pHout(hout)
	{		
		pConsoleScreenBuffer = new(CONSOLE_SCREEN_BUFFER_INFOEX);

		pConsoleScreenBuffer->cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);;
		//how many chars there are
		pConsoleScreenBuffer->dwSize = { width/4, height/2/4 };
		pConsoleScreenBuffer->dwCursorPosition = { 0, 0 };
		pConsoleScreenBuffer->wAttributes = COL_BACKGROUND | COL_PIXEL;
		//console coords
		pConsoleScreenBuffer->srWindow = { 0, 0, width/4, height/2/4 };
		pConsoleScreenBuffer->dwMaximumWindowSize = { 10000, 100000 };
		pConsoleScreenBuffer->wPopupAttributes = 0;
		pConsoleScreenBuffer->bFullscreenSupported = FALSE;
		
		SetPalette(PALETTE_DEFAULT);

		pScreenBuffer = new CHAR_INFO[width * (height*2)];
		CHAR_INFO chri = { ' ', 0 };

		for (int i = 0; i < width*height; i++)
			pScreenBuffer[i] = chri;
	}
	
	Screen::~Screen()
	{
		delete[] pScreenBuffer;
		delete pConsoleScreenBuffer;
	}


	void Screen::SetPixel(uint16_t x, uint16_t y, uint16_t color, uchar8_t chr)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;

		pScreenBuffer[x + y * width].Attributes = color;
		pScreenBuffer[x + y * width].Char.AsciiChar = chr;
	}

	void Screen::SetPixel(uint16_t x, uint16_t y, uint16_t color)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;

		uchar8_t chr = y % 2 == 0 ? 223 : 220;
		
		short mody = (short)round((y / 2.0));
		y -= mody;
				
		uchar8_t oldchr = pScreenBuffer[x + y * width].Char.AsciiChar;

		if (oldchr == 219 || oldchr == chr)
		{
			pScreenBuffer[x + y * width].Attributes = color;
			pScreenBuffer[x + y * width].Char.AsciiChar = oldchr;

			return;
		}

		if (oldchr != ' ')
			pScreenBuffer[x + y * width].Char.AsciiChar = 219;
		else
			pScreenBuffer[x + y * width].Char.AsciiChar = chr;
				
		pScreenBuffer[x + y * width].Attributes = color;
	}

	const uint16_t Screen::GetPixelColor(uint16_t x, uint16_t y)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return -1;

		return  pScreenBuffer[x + y * width].Attributes;
	}

	const char Screen::GetPixelChar(uint16_t x, uint16_t y)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return '\0';

		return  pScreenBuffer[x + y * width].Char.AsciiChar;
	}
	
	void Screen::SetPalette(const COLORREF palette[])
	{
		for (int i = 0; i < 16; i++)
			pConsoleScreenBuffer->ColorTable[i] = palette[i];
	}

	const COLORREF* Screen::GetPalette()
	{
		return pConsoleScreenBuffer->ColorTable;
	}

	void Screen::ClearFrame(uint16_t color)
	{
		for (int i = 0; i < width*height; i++)
		{
			pScreenBuffer[i].Attributes = color;
			pScreenBuffer[i].Char.AsciiChar = ' ';
		}
	}

	void Screen::DrawFrame()
	{		
		COORD screenbuffersize = { width, height };
		COORD screenbuffercoord = { 0, 0 };
		SMALL_RECT consolecoords = { 0, 0, width, height };

	/*	LockWindowUpdate(GetConsoleWindow());
		SendMessage(GetConsoleWindow(), WM_SETREDRAW, FALSE, 0);*/
		WriteConsoleOutputA(*pHout, pScreenBuffer, screenbuffersize, screenbuffercoord, &consolecoords);
	
		/*SendMessage(GetConsoleWindow(), WM_SETREDRAW, TRUE, 0);
		RedrawWindow(GetConsoleWindow(), 0, 0, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW);
		LockWindowUpdate(0);*/



	}

	uint16_t Screen::GetWidth()
	{
		return width / 4;
	}

	uint16_t Screen::GetHeight()
	{
		return height / 4;
	}

	CONSOLE_SCREEN_BUFFER_INFOEX* Screen::GetConsoleScreenBuffer()
	{
		return pConsoleScreenBuffer;
	}

}