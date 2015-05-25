#ifndef MESH_HPP
#define MESH_HPP

#include <stdint.h>

#include "Vertex.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"

namespace Render
{
	class Mesh
	{

	private:
			
		Vertex* pVertices;
		uint32_t vertices_c;
		uint32_t* pIndices;
		uint32_t indices_c;

		Transform transform;

	public:

		Mesh();
		Mesh(Vertex vertices[], uint32_t vertices_c, uint32_t indices[], uint32_t indices_c);
		~Mesh();

		void SetVertices(Vertex vertices[], uint32_t verticescount);
		void SetIndicies(uint32_t indices[], uint32_t indicescount);
		void CalculateNormals();
		
		Vertex* GetVertices();
		uint32_t GetVerticesCount();
		uint32_t* GetIndices();
		uint32_t GetIndicesCount();

		Vector GetTranslation();
		void SetTranslation(const Vector* pos);
		void SetTranslation(double x, double y, double z);

		Orientation GetOrientation();
		void SetOrientation(const Orientation* orientation);

		void RotatePitch(double theta);
		void RotateYaw(double theta);
		void RotateRoll(double theta);
		void Rotate(const Angle* ang);

		Vector GetScale();
		void SetScale(const Vector* vec);
		void SetScale(double x, double y, double z);
		
		void RenderMesh(Renderer* renderer, bool filled, uint16_t color);
		
	};
}

#endif