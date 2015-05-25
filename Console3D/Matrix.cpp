#include "Matrix.hpp"

#include <math.h>

#include "MathUtil.hpp"
#include "Vector.hpp"
#include "Angle.hpp"
#include "Camera.hpp"

using namespace Render;

namespace Math
{

	Matrix::Matrix()
	{
		SetMatrix(0.0);
	}

	Matrix::Matrix(double mat[4][4])
	{
		SetMatrix(mat);
	}

	Matrix::Matrix(double val)
	{
		SetMatrix(val);
	}

	double Matrix::GetAt(int x, int y) const
	{
		return mat[x][y];
	}

	void Matrix::SetAt(int x, int y, double val)
	{
		mat[x][y] = val;
	}

	void Matrix::SetMatrix(double val)
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
				mat[x][y] = val;
		}
	}

	void Matrix::SetMatrix(double mat[4][4])
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
				this->mat[x][y] = mat[x][y];
		}
	}

	double* Matrix::GetMatrix()
	{
		return &mat[0][0];
	}

	Matrix* Matrix::SetIdentityMatrix()
	{
		mat[0][0] = 1;	mat[0][1] = 0;	mat[0][2] = 0;	mat[0][3] = 0;
		mat[1][0] = 0;	mat[1][1] = 1;	mat[1][2] = 0;	mat[1][3] = 0;
		mat[2][0] = 0;	mat[2][1] = 0;	mat[2][2] = 1;	mat[2][3] = 0;
		mat[3][0] = 0;	mat[3][1] = 0;	mat[3][2] = 0;	mat[3][3] = 1;

		return this;
	}

	Matrix* Matrix::SetTranslationMatrix(double x, double y, double z)
	{
		mat[0][0] = 1;	mat[0][1] = 0;	mat[0][2] = 0;	mat[0][3] = x;
		mat[1][0] = 0;	mat[1][1] = 1;	mat[1][2] = 0;	mat[1][3] = y;
		mat[2][0] = 0;	mat[2][1] = 0;	mat[2][2] = 1;	mat[2][3] = z;
		mat[3][0] = 0;	mat[3][1] = 0;	mat[3][2] = 0;	mat[3][3] = 1;

		return this;
	}

	Matrix* Matrix::SetTranslationMatrix(const Vector* vec)
	{
		return SetTranslationMatrix(vec->x, vec->y, vec->z);
	}

	Matrix* Matrix::SetRotationMatrix(double pitch, double yaw, double roll)
	{
		double mx[4][4];
		double my[4][4];
		double mz[4][4];

		double rp = MathUtil::ToRad(pitch);
		double ry = MathUtil::ToRad(yaw);
		double rr = MathUtil::ToRad(roll);

		mx[0][0] = 1;		 mx[0][1] = 0;        mx[0][2] = 0;         mx[0][3] = 0;
		mx[1][0] = 0;		 mx[1][1] = cos(rp);  mx[1][2] = -sin(rp);  mx[1][3] = 0;
		mx[2][0] = 0;		 mx[2][1] = sin(rp);  mx[2][2] = cos(rp);   mx[2][3] = 0;
		mx[3][0] = 0;		 mx[3][1] = 0;        mx[3][2] = 0;         mx[3][3] = 1;
		   
		my[0][0] = cos(ry);  my[0][1] = 0;		  my[0][2] = -sin(ry);  my[0][3] = 0;
		my[1][0] = 0;        my[1][1] = 1;		  my[1][2] = 0;         my[1][3] = 0;
		my[2][0] = sin(ry);  my[2][1] = 0;		  my[2][2] = cos(ry);   my[2][3] = 0;
		my[3][0] = 0;        my[3][1] = 0;		  my[3][2] = 0;         my[3][3] = 1;
		   														  
		mz[0][0] = cos(rr);  mz[0][1] = -sin(rr); mz[0][2] = 0;		    mz[0][3] = 0;
		mz[1][0] = sin(rr);  mz[1][1] = cos(rr);  mz[1][2] = 0;		    mz[1][3] = 0;
		mz[2][0] = 0;        mz[2][1] = 0;        mz[2][2] = 1;		    mz[2][3] = 0;
		mz[3][0] = 0;        mz[3][1] = 0;        mz[3][2] = 0;		    mz[3][3] = 1;

		Matrix matx = Matrix(mx);
		Matrix maty = Matrix(my);
		Matrix matz = Matrix(mz);

		Matrix res = matz * (maty * matx);

		*this = res;

		return this;
	}

	Matrix* Matrix::SetRotationMatrix(const Angle* ang)
	{
		return SetRotationMatrix(ang->pitch, ang->yaw, ang->roll);
	}

	Matrix* Matrix::SetQuaternionRotation(Orientation* orientation)
	{
		Vector r = orientation->GetLeft();
		Vector f = orientation->GetForward();
		Vector u = orientation->GetUp();

		mat[0][0] = r.x; mat[0][1] = r.y; mat[0][2] = r.z; mat[0][3] = 0;
		mat[1][0] = u.x; mat[1][1] = u.y; mat[1][2] = u.z; mat[1][3] = 0;
		mat[2][0] = f.x; mat[2][1] = f.y; mat[2][2] = f.z; mat[2][3] = 0;
		mat[3][0] = 0;   mat[3][1] = 0;   mat[3][2] = 0;   mat[3][3] = 1;

		return this;
	}

	Matrix* Matrix::SetScaleMatrix(double x, double y, double z)
	{
		mat[0][0] = x;	mat[0][1] = 0;	mat[0][2] = 0;	mat[0][3] = 0;
		mat[1][0] = 0;	mat[1][1] = y;	mat[1][2] = 0;	mat[1][3] = 0;
		mat[2][0] = 0;	mat[2][1] = 0;	mat[2][2] = z;	mat[2][3] = 0;
		mat[3][0] = 0;	mat[3][1] = 0;	mat[3][2] = 0;	mat[3][3] = 1;

		return this;
	}
	Matrix* Matrix::SetScaleMatrix(const Vector* vec)
	{
		return SetScaleMatrix(vec->x, vec->y, vec->z);
	}

	Matrix* Matrix::SetScreenSpaceMatrix(int width, int height)
	{
		double hw = width / 2.0;
		double hh = height / 2.0;

		mat[0][0] = hw;	mat[0][1] = 0;	 mat[0][2] = 0;	mat[0][3] = hw;
		mat[1][0] = 0;	mat[1][1] = -hh; mat[1][2] = 0;	mat[1][3] = hh;
		mat[2][0] = 0;	mat[2][1] = 0;	 mat[2][2] = 1;	mat[2][3] = 0;
		mat[3][0] = 0;	mat[3][1] = 0;	 mat[3][2] = 0;	mat[3][3] = 1;

		return this;
	}

	Matrix* Matrix::SetPixelRatioMatrix(int x, int y)
	{
		double ratio = (double)x / y;
		SetScaleMatrix(1 / ratio, 1, 1);

		return this;
	}

	Matrix* Matrix::SetProjectionMatrix(int width, int height, double znear, double zfar, double fov)
	{
		double ar = (double)width / (double)height;
		double rfov = MathUtil::ToRad(fov);
		double tanhf = tan(rfov / 2.0);
		double zrange = znear - zfar;

		mat[0][0] = 1.0/(tanhf*ar);	mat[0][1] = 0;			 mat[0][2] = 0;						mat[0][3] = 0;
		mat[1][0] = 0;				mat[1][1] = 1.0 / tanhf; mat[1][2] = 0;						mat[1][3] = 0;
		mat[2][0] = 0;				mat[2][1] = 0;			 mat[2][2] = (-znear-zfar)/zrange;	mat[2][3] = 2 * zfar*znear/zrange;
		mat[3][0] = 0;				mat[3][1] = 0;			 mat[3][2] = 1;						mat[3][3] = 0;

		return this;
	}
	
}
