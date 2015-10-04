#ifndef TRIANGLEEDGE_HPP
#define TRIANGLEEDGE_HPP

#include "Vector.hpp"

using namespace Math;

namespace Render
{
	class TriangleEdge
	{

	private:

		Vector p1;
		double l1;

		Vector p2;
		double l2;

		double xslope;
		double zslope;
		double wslope;
		double lslope;

		double curx;
		double curz;
		double curw;
		double curl;

	public:

		TriangleEdge();
		TriangleEdge(const Vector* p1, double l1, const Vector* p2, double l2);
		~TriangleEdge();

		Vector GetP1();
		Vector GetP2();

		double GetStartY();
		double GetEndY();

		double GetCurrentX();
		double GetCurrentZ();
		double GetCurrentW();
		double GetCurrentLight();

		void Step();

	};
}


#endif