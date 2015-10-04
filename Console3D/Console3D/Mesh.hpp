#ifndef MESH_HPP
#define MESH_HPP

#include <stdint.h>

#include "Vertex.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"
#include "Quaternion.hpp"

#include "ShadowMap.hpp"
#include "BaseLight.hpp"

namespace Render
{
	class Mesh
	{

	private:
			
		Vertex* pVertices;
		int vertices_c;
		int* pIndices;
		int indices_c;

		Transform transform;

	public:

		Mesh();
		Mesh(Vertex vertices[], int vertices_c, int indices[], int indices_c);
		~Mesh();

		void SetVertices(Vertex vertices[], int verticescount);
		void SetIndicies(int indices[], int indicescount);
		void CalculateNormals();
		
		Vertex* GetVertices();
		int GetVerticesCount();
		int* GetIndices();
		int GetIndicesCount();

		Vector GetTranslation();
		void SetTranslation(const Vector* pos);
		void SetTranslation(double x, double y, double z);

		Quaternion GetRotation();
		void SetRotation(const Quaternion* qua);
		void Rotate(const Quaternion* qua);
		void RotatePitch(double theta);
		void RotateYaw(double theta);
		void RotateRoll(double theta);

		Vector GetScale();
		void SetScale(const Vector* vec);
		void SetScale(double x, double y, double z);
		
		void RenderColoredMesh(Renderer* renderer, uint8_t color, bool cull);
		void RenderAutoMesh(Renderer* renderer, bool cull);
		void RenderLitMesh(Renderer* renderer, bool cull);

		void RenderShadowMap(Renderer* renderer, BaseLight* light);
		
	};
}

#endif