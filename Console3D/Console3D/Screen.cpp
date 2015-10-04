#include "Screen.hpp"

#include <string.h>
#include <math.h>
#include <omp.h>

#include "Util.hpp"
#include "Color.hpp"

using namespace Base;

namespace Render
{

	Screen::Screen(short screenwidth, short screenheight, short fontwidth, short fontheight, const COLORREF* palette, const HANDLE* hout) :
		fontwidth(fontwidth), fontheight(fontheight), pHout(hout)
	{		
		xchars = screenwidth / fontwidth;
		ychars = screenheight / fontheight;
		width = xchars;
		height = ychars*2;

		consolescreenbuffer.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		//how many chars there are
		consolescreenbuffer.dwSize = { xchars, ychars };
		consolescreenbuffer.dwCursorPosition = { 0, 0 };
		consolescreenbuffer.wAttributes = COL_BACKGROUND;
		//console coords
		consolescreenbuffer.srWindow = { 0, 0, xchars, ychars};
		consolescreenbuffer.dwMaximumWindowSize = { 10000, 10000 };
		consolescreenbuffer.wPopupAttributes = 0;
		consolescreenbuffer.bFullscreenSupported = FALSE;
		
		SetPalette(palette);

		pScreenBuffer = new CHAR_INFO[width * height];
		CHAR_INFO chri = { ' ', 0 };

		for (int i = 0; i < width*height; i++)
			pScreenBuffer[i] = chri;
	}
	
	Screen::~Screen()
	{
		delete[] pScreenBuffer;
	}

	void Screen::SetPixel(short x, short y, uint8_t color, uchar8_t chr)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		
		pScreenBuffer[x + y * width].Attributes = color;
		pScreenBuffer[x + y * width].Char.AsciiChar = chr;
	}

	void Screen::SetPixel(short x, short y, uint8_t color)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		
		bool pair = y % 2 == 0;

		short mody = (short)round((y / 2.0));
		y -= mody;

		CHAR_INFO* pixel = &pScreenBuffer[x + y * width];

		uint8_t oldfg = pixel->Attributes & 0x0F;	
		uint8_t oldbg = pixel->Attributes & 0xF0;

		if (pair)		
			color = oldbg | color;	
		else
			color = color << 4 | oldfg;

		pixel->Attributes = color;

	}

	uint8_t Screen::GetPixelColor(short x, short y)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return -1;

		return (uint8_t)pScreenBuffer[x + y * width].Attributes;
	}

	uchar8_t Screen::GetPixelChar(short x, short y)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return '\0';

		return pScreenBuffer[x + y * width].Char.AsciiChar;
	}

	void Screen::WriteString(short x, short y, uint8_t color, const char* str)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;

		for (int i = 0; str[i] != '\0'; i++)
		{
			pScreenBuffer[x + y * width + i].Attributes = color;
			pScreenBuffer[x + y * width + i].Char.AsciiChar = str[i];
		}
	}

	void Screen::SetPalette(const COLORREF palette[])
	{
		for (int i = 0; i < 16; i++)
		{
			consolescreenbuffer.ColorTable[i] = palette[i];
			this->palette[i] = palette[i];
		}
	}

	const COLORREF* Screen::GetPalette()
	{
		return consolescreenbuffer.ColorTable;
	}

	void Screen::ClearFrame(uint8_t color)
	{
		//memset(pScreenBuffer, color, width*height*2);

		//#pragma omp parallel for
		for (int y = 0; y < ychars; y++)
		{
			for (int x = 0; x < xchars; x++)
			{
				pScreenBuffer[x + y * width].Attributes = 0;
				pScreenBuffer[x + y * width].Char.AsciiChar = (byte)223;
			}
		}
				
	}

	void Screen::DrawFrame()
	{		
		COORD screenbuffersize = { xchars, ychars };
		COORD screenbuffercoord = { 0, 0 };

		SMALL_RECT consolecoords = { 0, 0, xchars, ychars };
		WriteConsoleOutputA(*pHout, pScreenBuffer, screenbuffersize, screenbuffercoord, &consolecoords);
			
	/*	LockWindowUpdate(GetConsoleWindow());
		SendMessage(GetConsoleWindow(), WM_SETREDRAW, FALSE, 0);*/
		
	/*	consolecoords = { xchars / 2, 0, xchars, ychars/2 };
	/*SendMessage(GetConsoleWindow(), WM_SETREDRAW, TRUE, 0);
		RedrawWindow(GetConsoleWindow(), 0, 0, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_NOCHILDREN | RDW_UPDATENOW);
		LockWindowUpdate(0);*/
	}

	short Screen::GetWidth()
	{
		return width;
	}

	short Screen::GetHeight()
	{
		return height;
	}

	short Screen::GetFontWidth()
	{
		return fontwidth;
	}

	short Screen::GetFontHeight()
	{
		return fontheight;
	}

	CONSOLE_SCREEN_BUFFER_INFOEX* Screen::GetConsoleScreenBuffer()
	{
		return &consolescreenbuffer;
	}

}