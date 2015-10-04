#include "Frustum.hpp"

#include <math.h>
#include <stdint.h>

#include "Plane.hpp"

namespace Render
{

	Frustum::Frustum()
	{

	}
	/*
	
	0 - 00
	1 - 01
	2 - 02
	3 - 03
	4 - 10
	5 - 11
	6 - 12
	7 - 13
	8 - 20
	9 - 21
   10 - 22
   11 - 23
   12 - 30
   13 - 31
   14 - 32
   15 - 33

   0 - 00
   1 - 10
   2 - 20
   3 - 30
   4 - 01
   5 - 11
   6 - 21
   7 - 31
   8 - 02
   9 - 12
   10 - 22
   11 - 32
   12 - 03
   13 - 13
   14 - 23
   15 - 33
	
	*/
	Frustum::Frustum(Matrix* mvp)
	{
		float proj[16];
		float modl[16];
		float clip[16];
		float t;
	
		/* Extract the numbers for the RIGHT plane */
		Vector rightn = Vector(mvp->GetAt(0, 3) - mvp->GetAt(0, 0), mvp->GetAt(1, 3) - mvp->GetAt(1, 0), mvp->GetAt(2, 3) - mvp->GetAt(2, 0));
		double rightd = mvp->GetAt(3, 3) - mvp->GetAt(3, 0);
		double rightlen = rightn.GetLength();

		rightn.Normalize();
		
		right = Plane(&rightn, rightd);		
	}

	Frustum::~Frustum()
	{

	}

	bool Frustum::ClipTriangleLeft(const Vector* point)
	{
		return right.PointBehindPlane(point);
	}

}