#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#define PI 3.1415926535897932384626433832795
#define PI2 6.283185307179586476925286766559

#include "Vector.hpp"

namespace Math
{
	namespace MathUtil
	{
		
		inline double ToRad(double num)
		{
			return PI * num / 180.0;
		}

		inline double ToDeg(double num)
		{
			return 180.0 * num / PI;
		}

		inline double Dot4(double a1, double a2, double a3, double a4, double b1, double b2, double b3, double b4)
		{
			return a1 * b1 + a2 * b2 + a3 * b3 + a4 * b4;
		}

		inline double Clamp(double val, double min, double max)
		{
			return val < min ? min : val > max ? max : val;
		}

		inline double Interpolate(double val, double fmin, double fmax, double tmin, double tmax)
		{
			double fdist = fmax - fmin;
			double tdist = tmax - tmin;

			double scaled = (val - fmin) / fdist;

			return tmin + (scaled * tdist);
		}
	
		inline double TriangleArea(const Vector* p1, const Vector* p2, const Vector* p3)
		{
			double x1 = p2->x - p1->x;
			double y1 = p2->y - p1->y;

			double x2 = p3->x - p1->x;
			double y2 = p3->y - p1->y;

			return (x1 * y2 - x2 * y1);
		}

		inline double CalculateSlope(double amax, double amin, double bmax, double bmin)
		{
			double adist = amax - amin;
			double bdist = bmax - bmin;

			if (bdist == 0)
				bdist = 1.0;

			return adist / bdist;
		}

	}
}

#endif