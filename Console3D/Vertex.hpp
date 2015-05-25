#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <stdint.h>

#include "Vector.hpp"
#include "Matrix.hpp"

using namespace Math;

namespace Render
{
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
		Vector* GetNormal();
		void SetNormal(const Vector* normal);

		void PerspectiveDivide();

		bool CanDraw();

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