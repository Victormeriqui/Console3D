#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <stdint.h>
#include <Windows.h>

#include "Util.hpp"

using namespace Base;

namespace Render
{
	class Screen
	{

	private:

		uint16_t width;
		uint16_t height;
		HANDLE* pHout;

		CONSOLE_SCREEN_BUFFER_INFOEX* pConsoleScreenBuffer;
		CHAR_INFO* pScreenBuffer;

		COLORREF palette[16];

	public:

		Screen(uint16_t width, uint16_t height, HANDLE* hout);
		~Screen();

		uint16_t GetWidth();
		uint16_t GetHeight();

		void SetPixel(uint16_t x, uint16_t y, uint16_t color, uchar8_t chr);
		void SetPixel(uint16_t x, uint16_t y, uint16_t color);
		
		const uint16_t GetPixelColor(uint16_t x, uint16_t y);
		const char GetPixelChar(uint16_t x, uint16_t y);

		void SetPalette(const COLORREF palette[]);
		const COLORREF* GetPalette();

		void ClearFrame(uint16_t color);
		void DrawFrame();

		//For setting the active screen buffer ONLY		
		CONSOLE_SCREEN_BUFFER_INFOEX* GetConsoleScreenBuffer();

	};
}

#endif