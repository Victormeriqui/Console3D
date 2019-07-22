#include "Mesh.hpp"

#include <stdint.h>
#include <omp.h>

#include "Vertex.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Transform.hpp"
#include "Quaternion.hpp"
#include "BaseLight.hpp"

namespace Render
{

	Mesh::Mesh() :
		transform(Transform())
	{
		pVertices = nullptr;
		pIndices = nullptr;
		vertices_c = 0;
		indices_c = 0;
	}
	
	Mesh::Mesh(Vertex vertices[], int vertices_c, int indices[], int indices_c) :
		transform(Transform())
	{
		SetVertices(vertices, vertices_c);
		SetIndicies(indices, indices_c);
	}

	Mesh::~Mesh()
	{
		delete[] pVertices;
		delete[] pIndices;
	}

	void Mesh::SetVertices(Vertex vertices[], int verticescount)
	{
		pVertices = new Vertex[verticescount];
		vertices_c = verticescount;

		for (int i = 0; i < vertices_c; i++)
			*(pVertices + i) = vertices[i];
	}

	void Mesh::SetIndicies(int indices[], int indicescount)
	{
		pIndices = new int[indicescount];
		indices_c = indicescount;

		for (int i = 0; i < indices_c; i++)
			*(pIndices + i) = indices[i];
	}

	void Mesh::CalculateNormals()
	{
		for (int i = 0; i < indices_c - 3; i += 3)
		{
			Vertex* v1 = &pVertices[pIndices[i]];
			Vertex* v2 = &pVertices[pIndices[i + 1]];
			Vertex* v3 = &pVertices[pIndices[i + 2]];

			Vector* p1 = v1->GetPosition();
			Vector* p2 = v2->GetPosition();
			Vector* p3 = v3->GetPosition();

			Vector l1 = *p2 - *p1;
			Vector l2 = *p3 - *p1;

			Vector normal = l1.GetCrossProduct(&l2);
			normal.Normalize();

			Vector n1 = *v1->GetNormal();
			Vector n2 = *v2->GetNormal();
			Vector n3 = *v3->GetNormal();

			v1->SetNormal(&(n1 + normal));
			v2->SetNormal(&(n2 + normal));
			v3->SetNormal(&(n3 + normal));
		}

		for (int i = 0; i < vertices_c; i++)
		{
			pVertices[i].GetNormal()->Normalize();
		}
	}
	

	void Mesh::RenderAutoMesh(Renderer* renderer, bool cull)
	{
		if (pVertices == nullptr || pIndices == nullptr || indices_c == 0 || vertices_c == 0)
			return;
		
		#pragma omp parallel for
		for (int i = 0; i < indices_c; i += 3)
		{
			Vertex v1 = *(pVertices + pIndices[i]);
			Vertex v2 = *(pVertices + pIndices[i + 1]);
			Vertex v3 = *(pVertices + pIndices[i + 2]);

			renderer->DrawTriangle(&transform, &v1, &v2, &v3, (i%15)+1, false, cull, false);
		}

	}

	void Mesh::RenderColoredMesh(Renderer* renderer, uint8_t color, bool cull)
	{
		if (pVertices == nullptr || pIndices == nullptr || indices_c == 0 || vertices_c == 0)
			return;

		//#pragma omp parallel for
		for (int i = 0; i < indices_c; i += 3)
		{
			Vertex v1 = *(pVertices + pIndices[i]);
			Vertex v2 = *(pVertices + pIndices[i + 1]);
			Vertex v3 = *(pVertices + pIndices[i + 2]);

			renderer->DrawTriangle(&transform, &v1, &v2, &v3, color, false, cull, false);
		}

	}

	void Mesh::RenderLitMesh(Renderer* renderer, bool cull)
	{
		if (pVertices == nullptr || pIndices == nullptr || indices_c == 0 || vertices_c == 0)
			return;
		
		#pragma omp parallel for
		for (int i = 0; i < indices_c; i += 3)
		{
			Vertex v1 = *(pVertices + pIndices[i]);
			Vertex v2 = *(pVertices + pIndices[i + 1]);
			Vertex v3 = *(pVertices + pIndices[i + 2]);

			renderer->DrawTriangle(&transform, &v1, &v2, &v3, 0, true, cull, false);
		}

	}

	void Mesh::RenderShadowMap(Renderer* renderer, BaseLight* light)
	{
		if (pVertices == nullptr || pIndices == nullptr || indices_c == 0 || vertices_c == 0)
			return;

		#pragma omp parallel for
		for (int i = 0; i < indices_c; i += 3)
		{
			Vertex v1 = *(pVertices + pIndices[i]);
			Vertex v2 = *(pVertices + pIndices[i + 1]);
			Vertex v3 = *(pVertices + pIndices[i + 2]);

			renderer->DrawShadowMapTriangle(&transform, light, &v1, &v2, &v3);
		}
	}


	Vertex* Mesh::GetVertices()
	{
		return pVertices;
	}

	int Mesh::GetVerticesCount()
	{
		return vertices_c;
	}
	
	int* Mesh::GetIndices()
	{
		return pIndices;
	}

	int Mesh::GetIndicesCount()
	{
		return indices_c;
	}

	
	Vector Mesh::GetTranslation()
	{
		return transform.GetTranslation();
	}

	void Mesh::SetTranslation(const Vector* pos)
	{
		transform.SetTranslation(pos);
	}

	void Mesh::SetTranslation(double x, double y, double z)
	{
		transform.SetTranslation(x, y, z);
	}

	Quaternion Mesh::GetRotation()
	{
		return transform.GetRotation();
	}

	void Mesh::SetRotation(const Quaternion* qua)
	{
		transform.SetRotation(qua);
	}

	void Mesh::Rotate(const Quaternion* qua)
	{
		transform.Rotate(qua);
	}

	void Mesh::RotatePitch(double theta)
	{
		transform.RotatePitch(theta);
	}

	void Mesh::RotateYaw(double theta)
	{
		transform.RotateYaw(theta);
	}

	void Mesh::RotateRoll(double theta)
	{
		transform.RotateRoll(theta);
	}

	Vector Mesh::GetScale()
	{
		return transform.GetScale();
	}

	void Mesh::SetScale(const Vector* vec)
	{
		transform.SetScale(vec);
	}

	void Mesh::SetScale(double x, double y, double z)
	{
		transform.SetScale(x, y, z);
	}


}