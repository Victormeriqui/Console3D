#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <stdint.h>
#include <vector>

#include "Vertex.hpp"
#include "Vector.hpp"

namespace Game
{

	inline void GeneratePlane(Vertex** vertices, uint32_t* vertices_c, uint32_t** indices, uint32_t* indices_c)
	{
		*vertices = new Vertex[100];
		*vertices_c = 0;
		*indices = new uint32_t[400];
		*indices_c = 0;

		for (int z = 4; z > -5; z--)
		{
			for (int x = -3; x < 4; x++)
			{
				(*vertices)[(*vertices_c)++] = Vertex(x, -1, z);
			}
		}

		int width = 7;

		for (int z = 0; z < 9; z++)
		{
			for (int x = 0; x < 7 - 1; x++)
			{
				(*indices)[(*indices_c)++] = (width*z) + x;
				(*indices)[(*indices_c)++] = (width*z) + x + 1;
				(*indices)[(*indices_c)++] = (width*z) + x + width;
			}
			z++;
			for (int x = 0; x < 7 - 1; x++)
			{
				(*indices)[(*indices_c)++] = (width*z) + x;
				(*indices)[(*indices_c)++] = (width*z) + x + 1 - width;
				(*indices)[(*indices_c)++] = (width*z) + x + 1;
			}
			z--;
		}
	}

	inline void SubDivide(std::vector<Vector>* ret, Vector v1, Vector v2, Vector v3, uint8_t depth)
	{
		if (depth == 0)
		{
			ret->push_back(v1);
			ret->push_back(v2);
			ret->push_back(v3);
			
			return;
		}

		Vector m1 = v1.GetMiddle(&v2);
		Vector m2 = v2.GetMiddle(&v3);
		Vector m3 = v3.GetMiddle(&v1);
		
		m1.w = 1;
		m2.w = 1;
		m3.w = 1;

		m1.Normalize();
		m2.Normalize();
		m3.Normalize();
		
		SubDivide(ret, v1, m1, m3, depth - 1);
		SubDivide(ret, m1, v2, m2, depth - 1);
		SubDivide(ret, m3, m2, v3, depth - 1);
		SubDivide(ret, m1, m2, m3, depth - 1);

		return;
	}

	inline void GenerateSphere(Vertex** vertices, uint32_t* vertices_c, uint32_t** indices, uint32_t* indices_c, uint8_t depth)
	{
		Vector icoverts[12] =
		{
			Vector(-1, 0, 1, 1),
			Vector(1, 0, 1, 1),
			Vector(-1, 0, -1, 1),
			Vector(1, 0, -1, 1),
			Vector(0, 1, 1, 1),
			Vector(0, 1, -1, 1),
			Vector(0, -1, 1, 1),
			Vector(0, -1, -1, 1),
			Vector(1, 1, 0, 1),
			Vector(-1, 1, 0, 1),
			Vector(1, -1, 0, 1),
			Vector(-1, -1, 0, 1)
		};

		int icotris[20][3] =
		{
			{ 0, 4, 1 },
			{ 0, 9, 4 },
			{ 9, 5, 4 },
			{ 4, 5, 8 },
			{ 4, 8, 1 },
			{ 8, 10, 1 },
			{ 8, 3, 10 },
			{ 5, 3, 8 },
			{ 5, 2, 3 },
			{ 2, 7, 3 },
			{ 7, 10, 3 },
			{ 7, 6, 10 },
			{ 7, 11, 6 },
			{ 11, 0, 6 },
			{ 0, 1, 6 },
			{ 6, 1, 10 },
			{ 9, 0, 11 },
			{ 9, 11, 2 },
			{ 9, 2, 5 },
			{ 7, 2, 11 }
		};


		std::vector<Vector> triangles;

		for (uint16_t i = 0; i < 20; i++)
		{
			Vector vert1 = icoverts[icotris[i][0]];
			Vector vert2 = icoverts[icotris[i][1]];
			Vector vert3 = icoverts[icotris[i][2]];

			vert1.Normalize();
			vert2.Normalize();
			vert3.Normalize();
			
			SubDivide(&triangles, vert1, vert2, vert3, depth);
		}

		*vertices = new Vertex[triangles.size()];
		*vertices_c = 0;

		for (uint32_t i = 0; i < triangles.size(); i++)
			(*vertices)[(*vertices_c)++] = Vertex(&triangles[i], &triangles[i]);
		
		*indices = new uint32_t[triangles.size()];
		*indices_c = 0;

		for (uint32_t i = 0; i < triangles.size(); i++)
			(*indices)[(*indices_c)++] = (uint32_t)i;
	}

}

#endif