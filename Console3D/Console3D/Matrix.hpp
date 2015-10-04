#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector.hpp"
#include "Quaternion.hpp"

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
		void SetMatrix(double m[4][4]);
		double* GetMatrix();

		void SetIdentityMatrix();
		void SetTranslationMatrix(double x, double y, double z);
		void SetTranslationMatrix(const Vector* vec);
		void SetRotationMatrix(double pitch, double yaw, double roll);
		void SetRotationMatrix(const Vector* r, const Vector* u, const Vector* f);
		void SetScaleMatrix(double x, double y, double z);
		void SetScaleMatrix(const Vector* vec);
		void SetScreenSpaceMatrix(int width, int height, int fontwidth, int fontheight);
		void SetPixelRatioMatrix(int x, int y);
		void SetPerspectiveProjectionMatrix(int width, int height, double znear, double zfar, double fov);
		void SetOrthographicProjectionMatrix(double left, double right, double up, double down, double near, double far);
		void SetShadowMapBiasMatrix();

		bool GetInverseMatrix(Matrix* outmat);

		//rotation matrix ONLY, obviously
		Quaternion GetQuaternion();

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
			double x = mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w;
			double y = mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w;
			double z = mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w;
			double w = mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w;
			
			return Vector(x, y, z, w);
		}

	};
}

#endif