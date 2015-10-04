#include "Vector.hpp"

#include <math.h>

#include "Quaternion.hpp"

namespace Math
{
	
	Vector::Vector(double x, double y, double z, double w) :
		x(x), y(y), z(z), w(w)
	{
	}

	Vector::Vector(double x, double y, double z) :
		x(x), y(y), z(z), w(1)
	{
	}

	Vector::Vector() :
		x(0), y(0), z(0), w(1)
	{
	}

	Vector::~Vector()
	{
	}

	double Vector::GetLength()
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}

	Vector* Vector::Normalize()
	{
		double len = GetLength();

		x /= len;
		y /= len;
		z /= len;
		w /= len;
		
		return this;
	}

	double Vector::GetLength3Comp()
	{
		return sqrt(x*x + y*y + z*z);
	}

	Vector* Vector::Normalize3Comp()
	{
		double len = GetLength3Comp();

		x /= len;
		y /= len;
		z /= len;
		
		return this;
	}

	Vector Vector::GetNormalized() const
	{
		return *Vector(*this).Normalize();
	}

	double Vector::GetDistance(const Vector* vec)
	{
		return (*vec - *this).GetLength();
	}

	double Vector::GetDotProduct(const Vector* vec)
	{
		return x*vec->x + y*vec->y + z*vec->z + w*vec->w;
	}

	Vector Vector::GetCrossProduct(const Vector* vec)
	{
		double nx = y * vec->z - z * vec->y;
		double ny = z * vec->x - x * vec->z;
		double nz = x * vec->y - y * vec->x;

		//very important, w must be 0 so the normal calculation wont carry Ws to the final model transformation
		//this bug took me like 2 hours to find.....
		return Vector(nx, ny, nz, 0);
	}

	Vector Vector::GetDirection(const Vector* vec)
	{
		Vector ret = (*vec - *this);
		ret.Normalize();

		return ret;
	}

	Vector Vector::GetMiddle(const Vector* vec)
	{
		return (*this + *vec) / 2.0;
	}
	
	Vector Vector::GetRotated(Quaternion* qua)
	{
		Quaternion conjugate = qua->GetConjugated();

		Quaternion res = (*qua)*(*this)*conjugate;

		return Vector(res.x, res.y, res.z, 1.0);
	}


	Vector Vector::GetRotated(const Vector* qaxis, double theta)
	{
		double stheta = sin(-theta);
		double ctheta = cos(-theta);

		Vector quaaxis = *qaxis;

		Vector rx = GetCrossProduct(&(quaaxis*stheta));
		Vector ry = quaaxis * (GetDotProduct(&(quaaxis * (1 - ctheta))));
		Vector rz = *this * ctheta;

		return rx + rz + ry;
	}
	

	void Vector::PerspectiveDivide()
	{
		x /= w;
		y /= w;
		z /= w;			
	}

	void Vector::PerspectiveMultiply()
	{
		x *= w;
		y *= w;
		z *= w;
	}

	Vector Vector::Lerp(const Vector* dest, double val)
	{
		//return (*this) * (1 - val) + *dest * val;	
		return (((*dest) - (*this)) * val) + (*this);
	}

}