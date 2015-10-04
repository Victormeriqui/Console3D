#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Vector.hpp"
#include "Matrix.hpp"

using namespace Math;

namespace Render
{
	//solving the lerp equation to find the value that interpolates to the edge of the clipping plane
	inline double GetLerpValueToPosition(double lerpedpoint1, double lerpedpoint2, double start, double end)
	{
		//lerp equation:  lerpedpoint = v1*(1-L) + v2*L, where L = lerp val
		//solve for lerpedpoint = 1, get lerp val
		//L = (lerpedpoint-v1) / (lerpedpoint-v1)-(lerpedpoint-v2)
		//L = (1-v1) / (1-v1)-(1-v2)
		//lerpedpoint for clipping will be w

		double numerator = lerpedpoint1 - start;
		double denominator = numerator - (lerpedpoint2 - end);

		return numerator / denominator;
	}


	class Vertex
	{

	private:

		Vector pos;
		Vector normal;

	public:

		Vertex();
		Vertex(double x, double y, double z);
		Vertex(const Vector* pos);
		Vertex(double x, double y, double z, double nx, double ny, double nz);
		Vertex(const Vector* pos, const Vector* normal);
		~Vertex();

		Vector* GetPosition();
		void SetPosition(const Vector* vec);

		Vector* GetNormal();
		void SetNormal(const Vector* normal);

		void PerspectiveDivide();

		Vertex LerpPosition(Vertex* vert, double val);

		double GetCoord(int index);//0 - x 1 - y 2 - z

		Vertex operator* (const Matrix mat) const 
		{ 
			return Vertex(&(mat*pos));
		}
		
		Vertex operator*= (const Matrix mat)
		{
			pos = mat*pos;

			return *this;
		}
	};
}


#endif