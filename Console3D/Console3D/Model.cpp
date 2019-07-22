#include "Model.hpp"

#include <stdint.h>
#include <omp.h>

#include "Vertex.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Transform.hpp"
#include "Quaternion.hpp"
#include "MD2.hpp"
#include "Time.hpp"
#include "Util.hpp"

using namespace Base;

namespace Render
{

	Model::Model() :
		transform(Transform())
	{
		pVertices = nullptr;
		pFrames = nullptr;
		pIndices = nullptr;
		vertices_c = 0;
		triangles_c = 0;
		indices_c = 0;
	}

	Model::Model(const char* filepath)
	{
		FILE* pFile = fopen(filepath, "rb");

		if (pFile == NULL)
			return;

		MD2_Header header;

		fread(&header, sizeof(MD2_Header), 1, pFile);

		if (header.magicnumber != 844121161)
			return;

		if (header.version != 8)
			return;

		vertices_c = header.vertices_c;
		pVertices = new Vertex[header.vertices_c];
		pFrames = new Vertex*[header.frames_c];

		for (int i = 0; i < header.frames_c; i++)
		{
			pFrames[i] = new Vertex[header.vertices_c];
		}

		triangles = new MD2_Triangle[header.triangles_c];
		triangles_c = header.triangles_c;

		fseek(pFile, header.triangles_offset, SEEK_SET);
		fread(triangles, sizeof(MD2_Triangle), header.triangles_c, pFile);

		fseek(pFile, header.frames_offset, SEEK_SET);
		for (int i = 0; i < header.frames_c; i++)
		{
			Vertex* vertices = new Vertex[header.vertices_c];
			float scalex = 0, scaley = 0, scalez = 0;
			float translationx = 0, translationy = 0, translationz = 0;

			char name[16];

			fread(&scalex, sizeof(float), 1, pFile);
			fread(&scaley, sizeof(float), 1, pFile);
			fread(&scalez, sizeof(float), 1, pFile);

			fread(&translationx, sizeof(float), 1, pFile);
			fread(&translationy, sizeof(float), 1, pFile);
			fread(&translationz, sizeof(float), 1, pFile);

			fread(&name, sizeof(char), 16, pFile);

			for (int j = 0; j < header.vertices_c; j++)
			{
				unsigned char cposx = 0, cposy = 0, cposz = 0;

				fread(&cposx, sizeof(unsigned char), 1, pFile);
				fread(&cposy, sizeof(unsigned char), 1, pFile);
				fread(&cposz, sizeof(unsigned char), 1, pFile);

				double x = (double)((cposx * scalex) + translationx);
				double y = (double)((cposy * scaley) + translationy);
				double z = (double)((cposz * scalez) + translationz);

				vertices[j] = Vertex(x, y, z);

				fseek(pFile, sizeof(unsigned char), SEEK_CUR);
			}

			AddFrame(vertices);
		}

		fclose(pFile);

		int* indices = new int[header.triangles_c * 3];
		int indices_c = 0;
		for (int i = 0; i < header.triangles_c; i++)
		{
			indices[indices_c++] = triangles[i].vertex[0];
			indices[indices_c++] = triangles[i].vertex[1];
			indices[indices_c++] = triangles[i].vertex[2];
		}

		SetIndicies(indices, indices_c);


	}

	Model::~Model()
	{
		delete[] pFrames;
		delete[] pIndices;
	}

	void Model::PlayAnimation(Animation* anim)
	{
		if (anim->frames_c < 2)
			return;

		animating = true;
		animframe = 0;
		curanim = *anim;
		curframe = anim->frames[animframe];
		nextframe = anim->frames[animframe + 1];

		interpprog = 0;
		oldtime = Time::GetCurTime();
	}

	void Model::AddFrame(Vertex* vertices)
	{
		for (int i = 0; i < vertices_c; i++)
		{
			pFrames[frames_c][i] = vertices[i];
		}

		frames_c++;
	}

	void Model::SetIndicies(int indices[], int indicescount)
	{
		pIndices = new int[indicescount];
		indices_c = indicescount;

		for (int i = 0; i < indices_c; i++)
			*(pIndices + i) = indices[i];
	}

	void Model::CalculateNormals()
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

			Vector normal = -l1.GetCrossProduct(&l2);
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
		
		/*
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

		Vector nn1 = -normal;
		Vector nn2 = -normal;
		Vector nn3 = -normal;

		v1->SetNormal(&nn1);
		v2->SetNormal(&nn2);
		v3->SetNormal(&nn3);

		v1->GetNormal()->Normalize();
		v2->GetNormal()->Normalize();
		v3->GetNormal()->Normalize();
		*/
	}

	void Model::RenderShadowMap(Renderer* renderer, BaseLight* light)
	{

		if (pFrames == nullptr || pIndices == nullptr || indices_c == 0 || vertices_c == 0 || frames_c == 0)
			return;

		if (animating)
		{
			curtime = Time::GetCurTime();

			interpprog = (curtime - oldtime) / fps;

			if (interpprog > 1.0)
				interpprog = 1.0;
		}

		#pragma omp parallel for
		for (int i = 0; i < vertices_c; i++)
		{
			Vertex fv1 = *(pFrames[curframe] + i);
			Vertex tv1 = *(pFrames[nextframe] + i);
			pVertices[i] = Vertex(&MathUtil::InterpolateVectors(interpprog, fv1.GetPosition(), tv1.GetPosition()));
		}

		CalculateNormals();

		#pragma omp parallel for
		for (int i = 0; i < indices_c; i += 3)
		{
			Vertex v1 = *(pVertices + pIndices[i]);
			Vertex v2 = *(pVertices + pIndices[i + 1]);
			Vertex v3 = *(pVertices + pIndices[i + 2]);

			renderer->DrawShadowMapTriangle(&transform, light, &v1, &v2, &v3);
		}

		if (animating)
		{
			if ((curtime - oldtime) >= fps)
			{
				if (animframe + 1 >= curanim.frames_c - 1)
				{
					animating = false;
					PlayAnimation(&curanim);
					return;

				}

				animframe++;
				curframe = curanim.frames[animframe];
				nextframe = curanim.frames[animframe + 1];

				oldtime = curtime;
			}
		}

	}

	void Model::RenderLitModel(Renderer* renderer, bool cull)
	{	
		
		if (pFrames == nullptr || pIndices == nullptr || indices_c == 0 || vertices_c == 0 || frames_c == 0)
			return;

		if (animating)
		{
			curtime = Time::GetCurTime();

			interpprog = (curtime - oldtime)/fps;
			
			if (interpprog > 1.0)
				interpprog = 1.0;
		}

		#pragma omp parallel for
		for (int i = 0; i < vertices_c; i++)
		{
			Vertex fv1 = *(pFrames[curframe]+i);
			Vertex tv1 = *(pFrames[nextframe]+i);
			pVertices[i] = Vertex(&MathUtil::InterpolateVectors(interpprog, fv1.GetPosition(), tv1.GetPosition()));
		}
		
		CalculateNormals();
		
		#pragma omp parallel for
		for (int i = 0; i < indices_c; i += 3)
		{
			Vertex v1 = *(pVertices + pIndices[i]);
			Vertex v2 = *(pVertices + pIndices[i + 1]);
			Vertex v3 = *(pVertices + pIndices[i + 2]);

			renderer->DrawTriangle(&transform, &v1, &v2, &v3, 0, true, cull, true);
		}
		
		if (animating)
		{
			if ((curtime - oldtime) >= fps)
			{
				if (animframe+1 >= curanim.frames_c-1)
				{
					animating = false;
				//	PlayAnimation(&curanim);
					return;
					
				}

				animframe++;
				curframe = curanim.frames[animframe];
				nextframe = curanim.frames[animframe + 1];

				oldtime = curtime;
			}
		}
	
	}

	Vertex** Model::GetFrames()
	{
		return pFrames;
	}

	int Model::GetFrameCount()
	{
		return frames_c;
	}

	int Model::GetVertexCount()
	{
		return vertices_c;
	}

	int* Model::GetIndices()
	{
		return pIndices;
	}

	int Model::GetIndicesCount()
	{
		return indices_c;
	}


	Vector Model::GetTranslation()
	{
		return transform.GetTranslation();
	}

	void Model::SetTranslation(const Vector* pos)
	{
		transform.SetTranslation(pos);
	}

	void Model::SetTranslation(double x, double y, double z)
	{
		transform.SetTranslation(x, y, z);
	}

	Quaternion Model::GetRotation()
	{
		return transform.GetRotation();
	}

	void Model::SetRotation(const Quaternion* qua)
	{
		transform.SetRotation(qua);
	}

	void Model::Rotate(const Quaternion* qua)
	{
		transform.Rotate(qua);
	}

	void Model::RotatePitch(double theta)
	{
		transform.RotatePitch(theta);
	}

	void Model::RotateYaw(double theta)
	{
		transform.RotateYaw(theta);
	}

	void Model::RotateRoll(double theta)
	{
		transform.RotateRoll(theta);
	}

	Vector Model::GetScale()
	{
		return transform.GetScale();
	}

	void Model::SetScale(const Vector* vec)
	{
		transform.SetScale(vec);
	}

	void Model::SetScale(double x, double y, double z)
	{
		transform.SetScale(x, y, z);
	}


}