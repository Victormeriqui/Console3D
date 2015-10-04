#ifndef UTIL_HPP
#define UTIL_HPP

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <vector>

#include "Vertex.hpp"
#include "MathUtil.hpp"
#include "Vector.hpp"

using namespace Render;

namespace Base
{
	typedef unsigned char uchar8_t;
	
	inline void FindCenter(long int* x, long int* y)
	{
		RECT rect = { 0, 0, 0, 0 };
		GetWindowRect(GetConsoleWindow(), &rect);

		*x = rect.left + ((rect.right-rect.left)/2);
		*y = rect.top + ((rect.bottom - rect.top) / 2);
	}

	inline void GetMousePosition(long int* x, long int* y)
	{
		POINT coord = { 0, 0 };
		GetCursorPos(&coord);
		
		RECT rect = { 0, 0, 0, 0 };
		GetWindowRect(GetConsoleWindow(), &rect);

		*x = coord.x;
		*y = coord.y;
	}

	inline bool InFrustum(const Vector* pos)
	{
		double aw = fabs(pos->w);

		return fabs(pos->x) <= aw && fabs(pos->y) <= aw && fabs(pos->z) <= aw;
	}

	inline COLORREF LerpColors(double val, COLORREF color1, COLORREF color2)
	{
		uint8_t c1r = color1 & 0xFF0000 >> 16;
		uint8_t c1g = color1 & 0x00FF00 >> 8;
		uint8_t c1b = color1 & 0x0000FF;

		uint8_t c2r = color2 & 0xFF0000 >> 16;
		uint8_t c2g = color2 & 0x00FF00 >> 8;
		uint8_t c2b = color2 & 0x0000FF;

		uint8_t retr = MathUtil::LerpB(val, c1r, c2r);
		uint8_t retg = MathUtil::LerpB(val, c1g, c2g);
		uint8_t retb = MathUtil::LerpB(val, c1b, c2b);

		COLORREF ret = retb | retg | retr;
		
		return ret;
	}

	inline bool LoadModel(const char* filepath, Vertex** vertices, int* vertices_c, int** indices, int* indices_c)
	{
		FILE* pFile = fopen(filepath, "r");

		if (pFile == NULL)
			return false;

		std::vector<Vertex> verticesv;
		std::vector<int> indicesv;

		char line[1000];

		while (fgets(line, 1000, pFile) != NULL)
		{
			char* pCh;
			pCh = strtok(line, " ");

			if (!strcmp(pCh, "v"))
			{
				double vert[3];
				int vert_c = 0;
				pCh = strtok(NULL, " ");
				while (pCh != NULL && vert_c < 3)
				{
					sscanf(pCh, "%lf", &vert[vert_c++]);
					pCh = strtok(NULL, " ");
				}

				Vertex vertex = Vertex(vert[0], vert[1], vert[2]);
				verticesv.push_back(vertex);

			}
			else if (!strcmp(pCh, "f"))
			{
				int face[4];
				int face_c = 0;
				pCh = strtok(NULL, " ");
				while (pCh != NULL && strcmp(pCh, "\n") != 0)
				{
					sscanf(pCh, "%u", &face[face_c++]);
					pCh = strtok(NULL, " ");
				}

				if (face_c == 3)
				{
					indicesv.push_back(face[0]-1);
					indicesv.push_back(face[1]-1);
					indicesv.push_back(face[2]-1);
				}
				else
				{
					indicesv.push_back(face[0]-1);
					indicesv.push_back(face[2]-1);
					indicesv.push_back(face[3]-1);
				}

			}

		}

		*vertices_c = (int)verticesv.size();
		*indices_c = (int)indicesv.size();

		*vertices = new Vertex[*vertices_c];
		*indices = new int[*indices_c];

		std::copy(verticesv.begin(), verticesv.begin() + *vertices_c, *vertices);
		std::copy(indicesv.begin(), indicesv.begin() + *indices_c, *indices);

		return true;
	}

}

#endif