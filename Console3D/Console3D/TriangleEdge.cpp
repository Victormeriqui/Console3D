#include "TriangleEdge.hpp"

#include <math.h>

#include "Vector.hpp"
#include "MathUtil.hpp"

using namespace Math;

namespace Render
{

	TriangleEdge::TriangleEdge() : 
		p1(Vector()), l1(1), p2(Vector()), l2(1)
	{
		xslope = 0;
		zslope = 0;
		wslope = 0;
		lslope = 0;

		curx = 0;
		curz = 0;
		curl = 0;
	}
	
	TriangleEdge::TriangleEdge(const Vector* p1, double l1, const Vector* p2, double l2) :
		p1(*p1), l1(l1), p2(*p2), l2(l2)
	{
		xslope = MathUtil::CalculateSlope(p2->x, p1->x, p2->y, p1->y);
		zslope = MathUtil::CalculateSlope(p2->z, p1->z, p2->y, p1->y);
		wslope = MathUtil::CalculateSlope(p2->w, p1->w, p2->y, p1->y);
		lslope = MathUtil::CalculateSlope(l2, l1, p2->y, p1->y);

		curx = p1->x;
		curz = p1->z;
		curw = p1->w;
		curl = l1;
	}
	
	TriangleEdge::~TriangleEdge()
	{
	}

	Vector TriangleEdge::GetP1()
	{
		return p1;
	}

	Vector TriangleEdge::GetP2()
	{
		return p2;
	}

	void TriangleEdge::Step()
	{
		curx += xslope;
		curz += zslope;
		curw += wslope;
		curl += lslope;
	}
	
	double TriangleEdge::GetStartY()
	{
		return p1.y;
	}

	double TriangleEdge::GetEndY()
	{
		return p2.y;
	}
		
	double TriangleEdge::GetCurrentX()
	{
		return curx;
	}

	double TriangleEdge::GetCurrentZ()
	{
		return curz;
	}

	double TriangleEdge::GetCurrentW()
	{
		return curw;
	}

	double TriangleEdge::GetCurrentLight()
	{
		return curl;
	}

}