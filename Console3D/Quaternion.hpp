#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector.hpp"

namespace Math
{
	class Quaternion
	{

	public:

		double x;
		double y;
		double z;
		double w;

		Quaternion(double x, double y, double z, double w);
		Quaternion(Vector axis, double ang);
		Quaternion();
		~Quaternion();

		double GetLength();
		Quaternion* Normalize();
		Quaternion GetNormalized();

		Quaternion* Conjugate();
		Quaternion GetConjugated();

		Quaternion operator* (Quaternion qua) const 
		{ 
			double nx = x * qua.w + w * qua.x + y * qua.z - z * qua.y;
			double ny = y * qua.w + w * qua.y + z * qua.x - x * qua.z;
			double nz = z * qua.w + w * qua.z + x * qua.y - y * qua.x;
			double nw = w * qua.w - x * qua.x - y * qua.y - z * qua.z;

			return Quaternion(nx, ny, nz, nw);
		}

		Quaternion operator* (Vector vec) const
		{
			double nx = w * vec.x + y * vec.z - z * vec.y;
			double ny = w * vec.y + z * vec.x - x * vec.z;
			double nz = w * vec.z + x * vec.y - y * vec.x;
			double nw = -x * vec.x - y * vec.y - z * vec.z;

			return Quaternion(nx, ny, nz, nw);
		}

	};
}

#endif