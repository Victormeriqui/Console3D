#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <vector>

#include "Vertex.hpp"
#include "Vector.hpp"
#include "Noise.hpp"

namespace Game
{

	inline void GeneratePlane(Vertex** vertices, int* vertices_c, int** indices, int* indices_c, int width, int height, double scale)
	{
		*vertices = new Vertex[width * height];
		int verts_c = 0;
		*indices = new int[width * height * 6];
		int index_c = 0;

		double noisex = 0;
		double noisey = 0;

		for (int y = 0; y < height; y++)
		{
			noisex = 0;
			for (int x = 0; x < width; x++)
			{
				double nois = Math::Noise(noisex, noisey, 5);
				(*vertices)[verts_c++] = Vertex(x/scale, y/scale, nois, 0, 0, 0);
				
				noisex += 0.3;
			}
			noisey += 0.3;
		}
		int i = 0;
		int blaa = 0;
		for (int y = 0; y < height-1; y++)
		{
			for (int x = 0; x < width-1; x++)
			{
				blaa = i + width;
				(*indices)[index_c++] = blaa;
				blaa = i + 1;
				(*indices)[index_c++] = blaa;

				(*indices)[index_c++] = i;
			
			
				i++;
				blaa = i + width - 1;
				(*indices)[index_c++] = blaa;
				blaa = i + width;
				(*indices)[index_c++] = blaa;
				(*indices)[index_c++] = i;
		
		
			}
			i++;
		}

		*indices_c = index_c;
		*vertices_c = verts_c;
		/*
		int hwidth = width / 2;
		int hdepth = depth / 2;

		*vertices = new Vertex[width * depth];
		int verts_c = 0;
		*indices = new int[width * depth * 6];
		int index_c = 0;

		for (int z = -hdepth; z <= hdepth; z++)
		{
			for (int x = -hwidth; x <= hwidth; x++)
			{
				(*vertices)[verts_c++] = Vertex(x, 0, z, 0, 1, 0);
			}
		}

		*vertices_c = verts_c;
			
		int i = 0;
		for (int z = 0; z < depth-1; z++)
		{
			for (int x = 0; x < width; x++)
			{
				int index1 = 0, index2 = 0, index3 = 0;

				if (x != width-1)
				{
					index1 = i;
					index2 = i + 1;
					index3 = i + width;

					(*indices)[index_c++] = index3;
					(*indices)[index_c++] = index2;
					(*indices)[index_c++] = index1;
				}
				if (x != 0)
				{
					index1 = i;
					index2 = i + width - 1;
					index3 = i + width;											

					(*indices)[index_c++] = index1;
					(*indices)[index_c++] = index2;
					(*indices)[index_c++] = index3;
				}
				i++;
			}
		}
		*indices_c = index_c;
		*/
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

		m1.Normalize3Comp();
		m2.Normalize3Comp();
		m3.Normalize3Comp();
		
		SubDivide(ret, v1, m1, m3, depth - 1);
		SubDivide(ret, m1, v2, m2, depth - 1);
		SubDivide(ret, m3, m2, v3, depth - 1);
		SubDivide(ret, m1, m2, m3, depth - 1);

		return;
	}

	inline void GenerateSphere(Vertex** vertices, int* vertices_c, int** indices, int* indices_c, uint8_t depth)
	{
		Vector icoverts[12] =
		{
			Vector(-1, 0, 1),
			Vector(1, 0, 1),
			Vector(-1, 0, -1),
			Vector(1, 0, -1),
			Vector(0, 1, 1),
			Vector(0, 1, -1),
			Vector(0, -1, 1),
			Vector(0, -1, -1),
			Vector(1, 1, 0),
			Vector(-1, 1, 0),
			Vector(1, -1, 0),
			Vector(-1, -1, 0)
		};

		int icotris[20][3] =
		{
			{ 1, 4, 0 },
			{ 4, 9, 0 },
			{ 4, 5, 9 },
			{ 8, 5, 4 },
			{ 1, 8, 4 },
			{ 1, 10, 8 },
			{ 10, 3, 8 },
			{ 8, 3, 5 },
			{ 3, 2, 5 },
			{ 3, 7, 2 },
			{ 3, 10, 7 },
			{ 10, 6, 7 },
			{ 6, 11, 7 },
			{ 6, 0, 11 },
			{ 6, 1, 0 },
			{ 10, 1, 6 },
			{ 11, 0, 9 },
			{ 2, 11, 9 },
			{ 5, 2, 9 },
			{ 11, 2, 7 }
		};


		std::vector<Vector> triangles;

		for (int i = 0; i < 20; i++)
		{
			Vector vert1 = icoverts[icotris[i][0]];
			Vector vert2 = icoverts[icotris[i][1]];
			Vector vert3 = icoverts[icotris[i][2]];

			vert1.Normalize3Comp();
			vert2.Normalize3Comp();
			vert3.Normalize3Comp();
			
			SubDivide(&triangles, vert1, vert2, vert3, depth);
		}

		*vertices = new Vertex[triangles.size()];
		*vertices_c = 0;

		for (unsigned int i = 0; i < triangles.size(); i++)
			(*vertices)[(*vertices_c)++] = Vertex(&triangles[i], &Vector(0, 0, 0));
		
		*indices = new int[triangles.size()];
		*indices_c = 0;

		for (unsigned int i = 0; i < triangles.size(); i++)
			(*indices)[(*indices_c)++] = (int)i;
	}

}

#endif