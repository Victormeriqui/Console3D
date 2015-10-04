#include "Plane.hpp"

#include "Vector.hpp"

namespace Math
{

	Plane::Plane() :
		normal(Vector()), distance(0)
	{
	}

	Plane::Plane(const Vector* normal, double distance) :
		normal(*normal), distance(distance)
	{
	}

	Plane::Plane(const Vector* p1, const Vector* p2, const Vector* p3)
	{
		Vector p2p1 = *p2 - *p1;
		Vector p3p1 = *p3 - *p1;

		Vector cross = p2p1.GetCrossProduct(&p3p1);
		cross.Normalize();

		
		normal = cross;
		distance = cross.GetDotProduct(p1);
	}

	Plane::~Plane()
	{
	}

	Vector Plane::PlaneLineIntersection(Vector* p1, Vector* p2, bool* parallel)
	{
		/*
		plane line intersection, build ray from the 2 points
		ray: orig + dir*t
		orig = p1, dir = p2-p1:norm
		*/
		*parallel = false;

		Vector orig = *p1;
		Vector dir = p1->GetDirection(p2);
		double t = p1->GetDistance(p2);

		Vector diff = *p2 - *p1;
		double dotp1 = normal.GetDotProduct(p1);
		double dotdiff = normal.GetDotProduct(&diff);

		if (dotdiff == 0)
			*parallel = true;

		Vector res = *p1 + (diff * ((distance - dotp1) / dotdiff));

		return res;
	}

	bool Plane::PointBehindPlane(const Vector* point)
	{
		if (normal.x * point->x + normal.y * point->y + normal.z * point->z + distance <= 0)
			return true;

		return false;
	}

}