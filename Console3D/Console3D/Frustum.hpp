#ifndef Frustum_HPP
#define Frustum_HPP

#include <stdint.h>

#include "Plane.hpp"
#include "Vertex.hpp"
#include "Matrix.hpp"

using namespace Math;

namespace Render
{

	class Frustum
	{

	private :

		Plane right;
		Plane left;
	

	public:

		Frustum();
		Frustum(Matrix* mvp);
		~Frustum();

		bool ClipTriangleLeft(const Vector* point);

	};

}

#endif