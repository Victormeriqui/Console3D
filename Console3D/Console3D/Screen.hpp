#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <Windows.h>
#include <stdint.h>

#include "Util.hpp"

using namespace Base;

namespace Render
{
	class Screen
	{

	private:

		short xchars;
		short ychars;
		short width;
		short height;
		short fontwidth;
		short fontheight;

		const HANDLE* pHout;

		CONSOLE_SCREEN_BUFFER_INFOEX consolescreenbuffer;
		CHAR_INFO* pScreenBuffer;

		COLORREF palette[16];

	public:

		int triangles = 0;

		Screen(short screenwidth, short screenheight, short fontwidth, short fontheight, const COLORREF* palette, const HANDLE* hout);
		~Screen();

		short GetWidth();
		short GetHeight();

		short GetFontWidth();
		short GetFontHeight();

		void SetPixel(short x, short y, uint8_t color, uchar8_t chr);
		void SetPixel(short x, short y, uint8_t color);

		uint8_t GetPixelColor(short x, short y);
		uchar8_t GetPixelChar(short x, short y);
		
		void WriteString(short x, short y, uint8_t color, const char* str);

		void SetPalette(const COLORREF palette[]);
		const COLORREF* GetPalette();

		void ClearFrame(uint8_t color);
		void DrawFrame();

		//For setting the active screen buffer ONLY		
		CONSOLE_SCREEN_BUFFER_INFOEX* GetConsoleScreenBuffer();

	};
}

#endif