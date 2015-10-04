#include "Matrix.hpp"

#include <omp.h>
#include <string.h>
#include <math.h>

#include "MathUtil.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"

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
		memset(mat, (int)val, 16);
	}

	void Matrix::SetMatrix(double m[4][4])
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
				mat[x][y] = m[x][y];
		}
	}

	double* Matrix::GetMatrix()
	{
		return &mat[0][0];
	}

	void Matrix::SetIdentityMatrix()
	{
		mat[0][0] = 1;	mat[0][1] = 0;	mat[0][2] = 0;	mat[0][3] = 0;
		mat[1][0] = 0;	mat[1][1] = 1;	mat[1][2] = 0;	mat[1][3] = 0;
		mat[2][0] = 0;	mat[2][1] = 0;	mat[2][2] = 1;	mat[2][3] = 0;
		mat[3][0] = 0;	mat[3][1] = 0;	mat[3][2] = 0;	mat[3][3] = 1;
	}

	void Matrix::SetTranslationMatrix(double x, double y, double z)
	{
		mat[0][0] = 1;	mat[0][1] = 0;	mat[0][2] = 0;	mat[0][3] = x;
		mat[1][0] = 0;	mat[1][1] = 1;	mat[1][2] = 0;	mat[1][3] = y;
		mat[2][0] = 0;	mat[2][1] = 0;	mat[2][2] = 1;	mat[2][3] = z;
		mat[3][0] = 0;	mat[3][1] = 0;	mat[3][2] = 0;	mat[3][3] = 1;		
	}

	void Matrix::SetTranslationMatrix(const Vector* vec)
	{
		SetTranslationMatrix(vec->x, vec->y, vec->z);
	}

	void Matrix::SetRotationMatrix(double pitch, double yaw, double roll)
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

		*this = matz * (maty * matx);
	}
	
	void Matrix::SetRotationMatrix(const Vector* r, const Vector* u, const Vector* f)
	{	
		mat[0][0] = r->x; mat[0][1] = r->y; mat[0][2] = r->z; mat[0][3] = 0;
		mat[1][0] = u->x; mat[1][1] = u->y; mat[1][2] = u->z; mat[1][3] = 0;
		mat[2][0] = f->x; mat[2][1] = f->y; mat[2][2] = f->z; mat[2][3] = 0;
		mat[3][0] = 0;    mat[3][1] = 0;    mat[3][2] = 0;    mat[3][3] = 1;
	}


	void Matrix::SetScaleMatrix(double x, double y, double z)
	{
		mat[0][0] = x;	mat[0][1] = 0;	mat[0][2] = 0;	mat[0][3] = 0;
		mat[1][0] = 0;	mat[1][1] = y;	mat[1][2] = 0;	mat[1][3] = 0;
		mat[2][0] = 0;	mat[2][1] = 0;	mat[2][2] = z;	mat[2][3] = 0;
		mat[3][0] = 0;	mat[3][1] = 0;	mat[3][2] = 0;	mat[3][3] = 1;		
	}

	void Matrix::SetScaleMatrix(const Vector* vec)
	{
		SetScaleMatrix(vec->x, vec->y, vec->z);
	}

	void Matrix::SetScreenSpaceMatrix(int width, int height, int fontwidth, int fontheight)
	{
		double hw = width / 2.0;
		double hh = height / 2.0;
		double ar = (double)fontwidth / ((double)fontheight/2.0);

		mat[0][0] = hw;	mat[0][1] = 0;		 mat[0][2] = 0;	mat[0][3] = hw - 0.5f;
		mat[1][0] = 0;	mat[1][1] = -hh*ar;  mat[1][2] = 0;	mat[1][3] = hh - 0.5f;
		mat[2][0] = 0;	mat[2][1] = 0;		 mat[2][2] = 1;	mat[2][3] = 0;
		mat[3][0] = 0;	mat[3][1] = 0;		 mat[3][2] = 0;	mat[3][3] = 1;		
	}

	//unused, this is accounted for in the screeenspace matrix ^
	void Matrix::SetPixelRatioMatrix(int x, int y)
	{
		double ratio = (double)x / y;
		SetScaleMatrix(1 / ratio, 1, 1);	
	}
	
	void Matrix::SetPerspectiveProjectionMatrix(int width, int height, double znear, double zfar, double fov)
	{
		double ar = (double)width / (double)height;
		double rfov = MathUtil::ToRad(fov);
		double tanhf = tan(rfov / 2.0);
		double zrange = znear - zfar;

		mat[0][0] = 1.0/(tanhf*ar);	mat[0][1] = 0;			 mat[0][2] = 0;						mat[0][3] = 0;
		mat[1][0] = 0;				mat[1][1] = 1.0 / tanhf; mat[1][2] = 0;						mat[1][3] = 0;
		mat[2][0] = 0;				mat[2][1] = 0;			 mat[2][2] = (-znear-zfar)/zrange;	mat[2][3] = (2.0*zfar*znear)/zrange;
		mat[3][0] = 0;				mat[3][1] = 0;			 mat[3][2] = 1;						mat[3][3] = 0;		
	}

	void Matrix::SetOrthographicProjectionMatrix(double left, double right, double up, double down, double near, double far)
	{
		double width = right - left;
		double height = up - down;
		double depth = far - near;

		mat[0][0] = 2.0 / width;	mat[0][1] = 0;						mat[0][2] = 0;					mat[0][3] = -(right + left) / width;
		mat[1][0] = 0;				mat[1][1] = 2.0 / height;			mat[1][2] = 0;					mat[1][3] = -(up + down) / height;
		mat[2][0] = 0;				mat[2][1] = 0;						mat[2][2] = -2.0 / depth;	    mat[2][3] = -(far + near) / depth;
		mat[3][0] = 0;				mat[3][1] = 0;						mat[3][2] = 0;					mat[3][3] = 1;	
	}

	void Matrix::SetShadowMapBiasMatrix()
	{
		double hw = 200 / 2.0;
		double hh = 200 / 2.0;
		
		mat[0][0] = hw;	mat[0][1] = 0;		 mat[0][2] = 0;	mat[0][3] = hw - 0.5f;
		mat[1][0] = 0;	mat[1][1] = -hh*3.0;     mat[1][2] = 0;	mat[1][3] = hh - 0.5f;
		mat[2][0] = 0;	mat[2][1] = 0;		 mat[2][2] = 1;	mat[2][3] = 0;
		mat[3][0] = 0;	mat[3][1] = 0;		 mat[3][2] = 0;	mat[3][3] = 1;
	}

	Quaternion Matrix::GetQuaternion()
	{
		Quaternion ret = Quaternion();

		ret.w = sqrt(MathUtil::GetMax(0, 1 + mat[0][0] + mat[1][1] + mat[2][2])) / 2;
		ret.x = sqrt(MathUtil::GetMax(0, 1 + mat[0][0] - mat[1][1] - mat[2][2])) / 2;
		ret.y = sqrt(MathUtil::GetMax(0, 1 - mat[0][0] + mat[1][1] - mat[2][2])) / 2;
		ret.z = sqrt(MathUtil::GetMax(0, 1 - mat[0][0] - mat[1][1] + mat[2][2])) / 2;
		
		ret.x *= MathUtil::Sign(ret.x * (mat[2, 1] - mat[1, 2]));
		ret.y *= MathUtil::Sign(ret.y * (mat[0, 2] - mat[2, 0]));
		ret.z *= MathUtil::Sign(ret.z * (mat[1, 0] - mat[0, 1]));

		return ret;
	}

	//from: http://www.mesa3d.org/
	//i know how to do this with gauss-jordan, but this is probably faster than i could ever make it, and this is computed per triangle
	bool Matrix::GetInverseMatrix(Matrix* outmat)
	{
		double inv[4][4], det;

		inv[0][0] = mat[1][1] * mat[2][2] * mat[3][3] -
			mat[1][1] * mat[2][3] * mat[3][2] -
			mat[2][1] * mat[1][2] * mat[3][3] +
			mat[2][1] * mat[1][3] * mat[3][2] +
			mat[3][1] * mat[1][2] * mat[2][3] -
			mat[3][1] * mat[1][3] * mat[2][2];

		inv[1][0] = -mat[1][0] * mat[2][2] * mat[3][3] +
			mat[1][0] * mat[2][3] * mat[3][2] +
			mat[2][0] * mat[1][2] * mat[3][3] -
			mat[2][0] * mat[1][3] * mat[3][2] -
			mat[3][0] * mat[1][2] * mat[2][3] +
			mat[3][0] * mat[1][3] * mat[2][2];

		inv[2][0] = mat[1][0] * mat[2][1] * mat[3][3] -
			mat[1][0] * mat[2][3] * mat[3][1] -
			mat[2][0] * mat[1][1] * mat[3][3] +
			mat[2][0] * mat[1][3] * mat[3][1] +
			mat[3][0] * mat[1][1] * mat[2][3] -
			mat[3][0] * mat[1][3] * mat[2][1];

		inv[3][0] = -mat[1][0] * mat[2][1] * mat[3][2] +
			mat[1][0] * mat[2][2] * mat[3][1] +
			mat[2][0] * mat[1][1] * mat[3][2] -
			mat[2][0] * mat[1][2] * mat[3][1] -
			mat[3][0] * mat[1][1] * mat[2][2] +
			mat[3][0] * mat[1][2] * mat[2][1];

		inv[0][1] = -mat[0][1] * mat[2][2] * mat[3][3] +
			mat[0][1] * mat[2][3] * mat[3][2] +
			mat[2][1] * mat[0][2] * mat[3][3] -
			mat[2][1] * mat[0][3] * mat[3][2] -
			mat[3][1] * mat[0][2] * mat[2][3] +
			mat[3][1] * mat[0][3] * mat[2][2];

		inv[1][1] = mat[0][0] * mat[2][2] * mat[3][3] -
			mat[0][0] * mat[2][3] * mat[3][2] -
			mat[2][0] * mat[0][2] * mat[3][3] +
			mat[2][0] * mat[0][3] * mat[3][2] +
			mat[3][0] * mat[0][2] * mat[2][3] -
			mat[3][0] * mat[0][3] * mat[2][2];

		inv[2][1] = -mat[0][0] * mat[2][1] * mat[3][3] +
			mat[0][0] * mat[2][3] * mat[3][1] +
			mat[2][0] * mat[0][1] * mat[3][3] -
			mat[2][0] * mat[0][3] * mat[3][1] -
			mat[3][0] * mat[0][1] * mat[2][3] +
			mat[3][0] * mat[0][3] * mat[2][1];

		inv[3][1] = mat[0][0] * mat[2][1] * mat[3][2] -
			mat[0][0] * mat[2][2] * mat[3][1] -
			mat[2][0] * mat[0][1] * mat[3][2] +
			mat[2][0] * mat[0][2] * mat[3][1] +
			mat[3][0] * mat[0][1] * mat[2][2] -
			mat[3][0] * mat[0][2] * mat[2][1];

		inv[0][2] = mat[0][1] * mat[1][2] * mat[3][3] -
			mat[0][1] * mat[1][3] * mat[3][2] -
			mat[1][1] * mat[0][2] * mat[3][3] +
			mat[1][1] * mat[0][3] * mat[3][2] +
			mat[3][1] * mat[0][2] * mat[1][3] -
			mat[3][1] * mat[0][3] * mat[1][2];

		inv[1][2] = -mat[0][0] * mat[1][2] * mat[3][3] +
			mat[0][0] * mat[1][3] * mat[3][2] +
			mat[1][0] * mat[0][2] * mat[3][3] -
			mat[1][0] * mat[0][3] * mat[3][2] -
			mat[3][0] * mat[0][2] * mat[1][3] +
			mat[3][0] * mat[0][3] * mat[1][2];

		inv[2][2] = mat[0][0] * mat[1][1] * mat[3][3] -
			mat[0][0] * mat[1][3] * mat[3][1] -
			mat[1][0] * mat[0][1] * mat[3][3] +
			mat[1][0] * mat[0][3] * mat[3][1] +
			mat[3][0] * mat[0][1] * mat[1][3] -
			mat[3][0] * mat[0][3] * mat[1][1];

		inv[3][2] = -mat[0][0] * mat[1][1] * mat[3][2] +
			mat[0][0] * mat[1][2] * mat[3][1] +
			mat[1][0] * mat[0][1] * mat[3][2] -
			mat[1][0] * mat[0][2] * mat[3][1] -
			mat[3][0] * mat[0][1] * mat[1][2] +
			mat[3][0] * mat[0][2] * mat[1][1];

		inv[0][3] = -mat[0][1] * mat[1][2] * mat[2][3] +
			mat[0][1] * mat[1][3] * mat[2][2] +
			mat[1][1] * mat[0][2] * mat[2][3] -
			mat[1][1] * mat[0][3] * mat[2][2] -
			mat[2][1] * mat[0][2] * mat[1][3] +
			mat[2][1] * mat[0][3] * mat[1][2];

		inv[1][3] = mat[0][0] * mat[1][2] * mat[2][3] -
			mat[0][0] * mat[1][3] * mat[2][2] -
			mat[1][0] * mat[0][2] * mat[2][3] +
			mat[1][0] * mat[0][3] * mat[2][2] +
			mat[2][0] * mat[0][2] * mat[1][3] -
			mat[2][0] * mat[0][3] * mat[1][2];

		inv[2][3] = -mat[0][0] * mat[1][1] * mat[2][3] +
			mat[0][0] * mat[1][3] * mat[2][1] +
			mat[1][0] * mat[0][1] * mat[2][3] -
			mat[1][0] * mat[0][3] * mat[2][1] -
			mat[2][0] * mat[0][1] * mat[1][3] +
			mat[2][0] * mat[0][3] * mat[1][1];

		inv[3][3] = mat[0][0] * mat[1][1] * mat[2][2] -
			mat[0][0] * mat[1][2] * mat[2][1] -
			mat[1][0] * mat[0][1] * mat[2][2] +
			mat[1][0] * mat[0][2] * mat[2][1] +
			mat[2][0] * mat[0][1] * mat[1][2] -
			mat[2][0] * mat[0][2] * mat[1][1];

		det = mat[0][0] * inv[0][0] + mat[0][1] * inv[1][0] + mat[0][2] * inv[2][0] + mat[0][3] * inv[3][0];

		if (det == 0)
			return false;

		det = 1.0 / det;

		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				inv[y][x] = inv[y][x] * det;
			}
		}

		*outmat = Matrix(inv);

		return true;
	}
}
