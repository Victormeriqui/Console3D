#include "Quaternion.hpp"

#include <math.h>

namespace Math
{
	
	Quaternion::Quaternion(double x, double y, double z, double w) :
		x(x), y(y), z(z), w(w)
	{
	}

	Quaternion::Quaternion() :
		x(0), y(0), z(0), w(0)
	{
	}

	Quaternion::~Quaternion()
	{
	}

	double Quaternion::GetLength()
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}

	Quaternion* Quaternion::Normalize()
	{
		double len = GetLength();
		x /= len;
		y /= len;
		z /= len;
		w /= len;

		return this;
	}

	Quaternion Quaternion::GetNormalized()
	{
		return *Quaternion(*this).Normalize();
	}

	Quaternion* Quaternion::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
		
		return this;
	}

	Quaternion Quaternion::GetConjugated()
	{
		return *Quaternion(*this).Conjugate();
	}

}