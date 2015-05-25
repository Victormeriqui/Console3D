#include "Mesh.hpp"

#include "Vertex.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Transform.hpp"
#include "Color.hpp"

namespace Render
{

	Mesh::Mesh()
	{
		pVertices = nullptr;
		pIndices = nullptr;
		vertices_c = 0;
		indices_c = 0;
	}
	
	Mesh::Mesh(Vertex vertices[], uint32_t vertices_c, uint32_t indices[], uint32_t indices_c)
	{
		SetVertices(vertices, vertices_c);
		SetIndicies(indices, indices_c);
	}

	Mesh::~Mesh()
	{
		delete[] pVertices;
		delete[] pIndices;
	}

	void Mesh::SetVertices(Vertex vertices[], uint32_t verticescount)
	{
		pVertices = new Vertex[verticescount];
		vertices_c = verticescount;

		for (uint16_t i = 0; i < vertices_c; i++)
			*(pVertices + i) = vertices[i];
	}

	void Mesh::SetIndicies(uint32_t indices[], uint32_t indicescount)
	{
		pIndices = new uint32_t[indicescount];
		indices_c = indicescount;

		for (uint16_t i = 0; i < indices_c; i++)
			*(pIndices + i) = indices[i];
	}

	void Mesh::CalculateNormals()
	{
		for (uint16_t i = 0; i < indices_c; i += 3)
		{
			uint32_t idx1 = pIndices[i];
			uint32_t idx2 = pIndices[i + 1];
			uint32_t idx3 = pIndices[i + 2];

			Vertex* v1 = &pVertices[idx1];
			Vertex* v2 = &pVertices[idx2];
			Vertex* v3 = &pVertices[idx3];

			Vector* p1 = v1->GetPosition();
			Vector* p2 = v2->GetPosition();
			Vector* p3 = v3->GetPosition();

			Vector* n1 = v1->GetNormal();
			Vector* n2 = v2->GetNormal();
			Vector* n3 = v3->GetNormal();

			Vector l1 = *p2 - *p1;
			Vector l2 = *p3 - *p1;

			Vector normal = l1.GetCrossProduct(&l2);
			normal.Normalize();

			Vector nn1 = (*n1) + normal;
			Vector nn2 = (*n2) + normal;
			Vector nn3 = (*n3) + normal;

			v1->SetNormal(&nn1);
			v2->SetNormal(&nn2);
			v3->SetNormal(&nn3);
		}

		for (uint16_t i = 0; i < vertices_c; i++)
		{
			pVertices[i].GetNormal()->Normalize();
		}
	}
	
	void Mesh::RenderMesh(Renderer* renderer, bool filled, uint16_t color)
	{
		if (pVertices == nullptr || pIndices == nullptr || indices_c == 0 || vertices_c == 0)
			return;

		uint16_t col = 1;
		
		uint16_t i2 = 0;
		for (uint16_t i = 0; i < indices_c;)
		{
			Vertex v1 = *(pVertices + pIndices[i]);
			Vertex v2 = *(pVertices + pIndices[i + 1]);
			Vertex v3 = *(pVertices + pIndices[i + 2]);

			if (filled)
			{
				if (color == 16)
					renderer->DrawLightningTriangle(&transform, &v1, &v2, &v3);
				else
					renderer->DrawTriangle(&transform, &v1, &v2, &v3, color == 17 || color == 18 ? col : color);
			}
			else
				renderer->DrawLineTriangle(&transform, &v1, &v2, &v3, color == 17 || color == 18 ? col : color);

			i2++;

			if (color == 18)
			{
				if (i2 % 2 == 0)
					col++;
			}
			else
				col++;
		
			if (col > 15)
				col = 1;

			i += 3;
		}
	}

	Vertex* Mesh::GetVertices()
	{
		return pVertices;
	}

	uint32_t Mesh::GetVerticesCount()
	{
		return vertices_c;
	}
	
	uint32_t* Mesh::GetIndices()
	{
		return pIndices;
	}

	uint32_t Mesh::GetIndicesCount()
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

	Orientation Mesh::GetOrientation()
	{
		return transform.GetOrientation();
	}

	void Mesh::SetOrientation(const Orientation* orientation)
	{
		transform.SetOrientation(orientation);
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

	void Mesh::Rotate(const Angle* ang)
	{
		transform.Rotate(ang);
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