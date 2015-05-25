#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "MathUtil.hpp"

#include "Vector.hpp"
#include "Angle.hpp"
#include "Orientation.hpp"

namespace Math
{
	class Matrix
	{

	private:

		double mat[4][4];

	public:
		
		Matrix();
		Matrix(double mat[4][4]);
		Matrix(double val);

		double GetAt(int x, int y) const;
		void SetAt(int x, int y, double val);
		void SetMatrix(double val);
		void SetMatrix(double mat[4][4]);
		double* GetMatrix();

		Matrix* SetIdentityMatrix();
		Matrix* SetTranslationMatrix(double x, double y, double z);
		Matrix* SetTranslationMatrix(const Vector* vec);
		Matrix* SetRotationMatrix(double pitch, double yaw, double roll);
		Matrix* SetRotationMatrix(const Angle* ang);
		Matrix* SetQuaternionRotation(Orientation* orientation);
		Matrix* SetScaleMatrix(double x, double y, double z);
		Matrix* SetScaleMatrix(const Vector* vec);
		Matrix* SetScreenSpaceMatrix(int width, int height);
		Matrix* SetPixelRatioMatrix(int x, int y);
		Matrix* SetProjectionMatrix(int width, int height, double znear, double zfar, double fov);

		Matrix operator* (Matrix omat) const
		{
			Matrix ret = Matrix();

			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{	
					ret.SetAt(y, x, mat[y][0] * omat.GetAt(0, x) +
									mat[y][1] * omat.GetAt(1, x) +
									mat[y][2] * omat.GetAt(2, x) +
									mat[y][3] * omat.GetAt(3, x));
				}
			}

			return ret;
		}

		Vector operator* (Vector vec) const
		{
			return Vector(
				mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w,
				mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w,
				mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w,
				mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w);
		}

	};
}

#endif