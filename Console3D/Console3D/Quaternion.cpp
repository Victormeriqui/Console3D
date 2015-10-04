#include "Quaternion.hpp"

#include <math.h>

#include "Vector.hpp"
#include "Matrix.hpp"

namespace Math
{
	
	Quaternion::Quaternion() :
		x(0), y(0), z(0), w(1)
	{
	}

	Quaternion::Quaternion(double x, double y, double z, double w) :
		x(x), y(y), z(z), w(w)
	{
	}
	
	Quaternion::Quaternion(const Vector* axis, double theta)
	{
		double shtheta = sin(theta / 2.0);
		double chtheta = cos(theta / 2.0);

		x = axis->x * shtheta;
		y = axis->y * shtheta;
		z = axis->z * shtheta;
		w = chtheta;
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

	Matrix Quaternion::GetRotationMatrix()
	{
		/*Vector f = Vector(2.0 * (x * z - w * y), 2.0 * (y * z + w * x), 1.0 - 2.0 * (x * x + y * y));
		Vector u = Vector(2.0 * (x * y + w * z), 1.0 - 2.0 * (x * x + z * z), 2.0 * (y * z - w * x));		
		Vector r = Vector(1.0 - 2.0 * (y * y + z * z), 2.0 * (x * y - w * z), 2.0 * (x * z + w * y));

		double mat[4][4];

		mat[0][0] = r.x; mat[0][1] = r.y; mat[0][2] = r.z; mat[0][3] = 0;
		mat[1][0] = u.x; mat[1][1] = u.y; mat[1][2] = u.z; mat[1][3] = 0;
		mat[2][0] = f.x; mat[2][1] = f.y; mat[2][2] = f.z; mat[2][3] = 0;
		mat[3][0] = 0;   mat[3][1] = 0;   mat[3][2] = 0;   mat[3][3] = 1;

		return Matrix(mat);
		*/
	
		double mat[4][4];

		double xx = x*x;
		double yy = y*y;
		double zz = z*z;
		double ww = w*w;
		double x2 = x*2.0;
		double y2 = y*2.0;
		double z2 = z*2.0;
		double w2 = w*2.0;
		double xy = x2 * y;
		double xz = x2 * z;
		double yz = y2 * z;
		double wx = w2 * x;
		double wy = w2 * y;
		double wz = w2 * z;

		mat[0][0] = ww+xx-yy-zz;			mat[0][1] = xy-wz;					mat[0][2] = xz+wy;			mat[0][3] = 0;
		mat[1][0] = xy+wz;					mat[1][1] = ww-xx+yy-zz;			mat[1][2] = yz-wx;			mat[1][3] = 0;
		mat[2][0] = xz-wy;					mat[2][1] = yz+wx;					mat[2][2] = ww-xx-yy+zz;	mat[2][3] = 0;
		mat[3][0] = 0;						mat[3][1] = 0;						mat[3][2] = 0;				mat[3][3] = 1;

		return Matrix(mat);
	}

	Vector Quaternion::GetForward()
	{
		return Vector(0, 0, 1, 1).GetRotated(this);
	}

	Vector Quaternion::GetBack()
	{
		return Vector(0, 0, -1, 1).GetRotated(this);
	}

	Vector Quaternion::GetUp()
	{
		return Vector(0, 1, 0, 1).GetRotated(this);
	}

	Vector Quaternion::GetDown()
	{
		return Vector(0, -1, 0, 1).GetRotated(this);
	}

	Vector Quaternion::GetRight()
	{
		return Vector(1, 0, 0, 1).GetRotated(this);
	}

	Vector Quaternion::GetLeft()
	{
		return Vector(-1, 0, 0, 1).GetRotated(this);
	}

}