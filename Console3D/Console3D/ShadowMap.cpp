#include "ShadowMap.hpp"

#include <limits.h>

namespace Render
{

	ShadowMap::ShadowMap() :
		width(100), height(100)
	{
		pDepthMap = new double[width*height];
	}

	ShadowMap::ShadowMap(int width, int height) :
		width(width), height(height)
	{
		pDepthMap = new double[width*height];
	}

	ShadowMap::~ShadowMap()
	{
		if(pDepthMap != nullptr)
			delete[] pDepthMap;
	}

	void ShadowMap::SetDepthPoint(int x, int y, double depth)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;

		pDepthMap[x + y * width] = depth;
	}

	double ShadowMap::GetDepthPoint(int x, int y)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return -1;

		return pDepthMap[x + y * width];
	}

	void ShadowMap::ClearDepthMap()
	{
		for (int i = 0; i < width*height; i++)
			pDepthMap[i] = INT_MAX;
				
	}

	int ShadowMap::GetWidth()
	{
		return width;
	}

	int ShadowMap::GetHeight()
	{
		return height;
	}

}