#ifndef PLANE_HPP
#define PLANE_HPP

#include "Vector.hpp"

namespace Math
{

	class Plane
	{

	private:

		Vector normal;
		double distance;

	public:

		Plane();
		Plane(const Vector* normal, double distance);
		Plane(const Vector* p1, const Vector* p2, const Vector* p3);
		~Plane();

		Vector PlaneLineIntersection(Vector* p1, Vector* p2, bool* parallel);
		bool PointBehindPlane(const Vector* point);

	};

}

#endif
