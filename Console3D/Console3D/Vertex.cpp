#include "Vertex.hpp"

#include <math.h>

#include "Vector.hpp"

using namespace Math;

namespace Render
{

	Vertex::Vertex() :
		pos(Vector(0, 0, 0, 1)), normal(Vector())
	{
	}

	Vertex::Vertex(double x, double y, double z) :
		pos(Vector(x, y, z, 1)), normal(Vector())
	{
		
	}

	Vertex::Vertex(const Vector* pos) :
		pos(*pos), normal(Vector())
	{
		
	}

	Vertex::Vertex(double x, double y, double z, double nx, double ny, double nz) :
		pos(Vector(x, y, z, 1)), normal(Vector(nx, ny, nz, 0))
	{

	}

	Vertex::Vertex(const Vector* pos, const Vector* normal) :
		pos(*pos), normal(*normal)
	{

	}

	Vertex::~Vertex()
	{

	}

	Vector* Vertex::GetPosition()
	{
		return &pos;
	}

	void Vertex::SetPosition(const Vector* vec)
	{
		pos = *vec;
	}
	
	Vector* Vertex::GetNormal()
	{
		return &normal;
	}

	void Vertex::SetNormal(const Vector* norm)
	{
		normal = *norm;
	}

	void Vertex::PerspectiveDivide()
	{
		pos.x /= pos.w;
		pos.y /= pos.w;
		pos.z /= pos.w;
	}

	Vertex Vertex::LerpPosition(Vertex* vert, double val)
	{
		Vector npos = pos.Lerp(vert->GetPosition(), val);
		Vector nnorm = normal.Lerp(vert->GetNormal(), val);

		return Vertex(&npos, &nnorm);
	}

	double Vertex::GetCoord(int index)
	{
		if (index == 0)
			return pos.x;
		if (index == 1)
			return pos.y;
		if (index == 2)
			return pos.z;

		return 0;
	}

}