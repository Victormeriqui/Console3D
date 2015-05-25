#include "Vector.hpp"

#include <math.h>

#include "MathUtil.hpp"
#include "Quaternion.hpp"

namespace Math
{
	
	Vector::Vector(double x, double y, double z, double w) :
		x(x), y(y), z(z), w(w)
	{
	}

	Vector::Vector(double x, double y, double z) :
		x(x), y(y), z(z), w(0)
	{
	}

	Vector::Vector() :
		x(0), y(0), z(0), w(0)
	{
	}

	Vector::~Vector()
	{
	}

	double Vector::GetLength()
	{
		return sqrt(x*x + y*y + z*z);
	}

	Vector* Vector::Normalize()
	{
		double len = GetLength();
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
		return x*vec->x + y*vec->y + z*vec->z;
	}

	Vector Vector::GetCrossProduct(const Vector* vec)
	{
		double nx = y * vec->z - z * vec->y;
		double ny = z * vec->x - x * vec->z;
		double nz = x * vec->y - y * vec->x;

		return Vector(nx, ny, nz);
	}

	Vector Vector::GetDirection(const Vector* vec)
	{
		Vector ret = (*vec - *this);
		ret.Normalize();

		return ret;
	}

	Vector Vector::GetMiddle(const Vector* vec)
	{
		return (*this + *vec) / 2;
	}

	Vector Vector::GetRotated(const Vector* qaxis, double theta)
	{
		Vector ret = *this;

		ret.Rotate(qaxis, theta);

		return ret;
	}

	Vector* Vector::Rotate(const Vector* qaxis, double theta)
	{
		double sinh = sin(MathUtil::ToRad(theta / 2.0));
		double cosh = cos(MathUtil::ToRad(theta / 2.0));

		double rx = qaxis->x * sinh;
		double ry = qaxis->y * sinh;
		double rz = qaxis->z * sinh;
		double rw = cosh;

		Quaternion rot = Quaternion(rx, ry, rz, rw);

		Quaternion ret = rot * (*this);
		ret * rot.GetConjugated();

		x = ret.x;
		y = ret.y;
		z = ret.z;

		return this;
	}

	void Vector::PerspectiveDivide()
	{
		x /= w;
		y /= w;
		z /= w;
		
	}

}