#ifndef UTIL_HPP
#define UTIL_HPP

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include "Vertex.hpp"

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

	inline bool LoadModel(const char* filepath, Vertex** vertices, uint32_t* vertices_c, uint32_t** indices, uint32_t* indices_c)
	{
		FILE* pFile = fopen(filepath, "r");

		if (pFile == NULL)
			return false;

		std::vector<Vertex> verticesv;
		std::vector<uint32_t> indicesv;

		char line[1000];

		while (fgets(line, 1000, pFile) != NULL)
		{
			char* pCh;
			pCh = strtok(line, " ");

			if (!strcmp(pCh, "v"))
			{
				double vert[3];
				uint32_t vert_c = 0;
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
				uint32_t face[4];
				uint32_t face_c = 0;
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

		*vertices_c = verticesv.size();
		*indices_c = indicesv.size();

		*vertices = new Vertex[*vertices_c];
		*indices = new uint32_t[*indices_c];

		std::copy(verticesv.begin(), verticesv.begin() + *vertices_c, *vertices);
		std::copy(indicesv.begin(), indicesv.begin() + *indices_c, *indices);

		return true;
	}

}

#endif