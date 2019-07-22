#include "Renderer.hpp"

#include <omp.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "Scene.hpp"
#include "Screen.hpp"
#include "Vector.hpp"
#include "Vertex.hpp"
#include "Util.hpp"
#include "Matrix.hpp"
#include "TriangleEdge.hpp"
#include "Camera.hpp"
#include "LightingManager.hpp"
#include "MathUtil.hpp"

namespace Render
{
	Renderer::Renderer()
	{
	}

	Renderer::Renderer(Screen* screen, double znear, double zfar) :
		pScreen(screen)
	{
		width = screen->GetWidth();
		height = screen->GetHeight();

		pDepthBuffer = new double[width*height];
		ClearDepthBuffer();

		pCamera = new Camera(width, height, znear, zfar, 90);
		screenspacem = Matrix();
		screenspacem.SetScreenSpaceMatrix(width, height, screen->GetFontWidth(), screen->GetFontHeight());

		lightingmanager = LightingManager();
	}

	Renderer::~Renderer()
	{
		delete[] pDepthBuffer;
		
		delete pCamera;
		//delete pLightingManager;
	}

	short Renderer::ScrW()
	{
		return width;
	}

	short Renderer::ScrH()
	{
		return height;
	}
	
	LightingManager* Renderer::GetLightingManager()
	{
		return &lightingmanager;
	}

	void Renderer::SetLightingManager(LightingManager* lman)
	{
		lightingmanager = *lman;
	}

	Camera* Renderer::GetCamera()
	{
		return pCamera;
	}

	void Renderer::SetCamera(Camera* cam)
	{
		pCamera = cam;
	}

	Screen* Renderer::GetScreen()
	{
		return pScreen;
	}

	void Renderer::SetScreen(Screen* screen)
	{
		pScreen = screen;
	}

	void Renderer::ClearDepthBuffer()
	{
		//
		//serial : 0000 32
		//for : 0000 10 - 0000 23
		
		//char timestr[10];
		//double time = omp_get_wtime();
		
		//#pragma omp parallel for
		for (int i = 0; i < (width*height); i++)
			pDepthBuffer[i] = INT_MAX;

		//snprintf(timestr, 10, "%lf", omp_get_wtime() - time);
		//pScreen->WriteString(0, 0, 15, timestr); 
	}

	void Renderer::DrawPoint(const Vector* point, uint8_t color)
	{
		short x = (short)point->x;
		short y = (short)point->y;
		double z = point->z;

		if (x < 0 || x >= width || y < 0 || y >= height)
			return;
		
		double cz = pDepthBuffer[x + y * width];
		
		if (cz < z)
			return;

     	pDepthBuffer[x + y * width] = z;

	//	pScreen->SetPixel(x, y, color, 219);
		pScreen->SetPixel(x, y, color);
	}

	void Renderer::DrawPoint(Transform* transform, Vertex* vert, uint8_t color)
	{
		Vector pos = *(vert->GetPosition());

		ModelTransform(transform, &pos); ViewTransform(transform, &pos); ProjectionTransform(transform, &pos);
		
		if(Base::InFrustum(&pos))
			DrawPoint(&pos, color);
	}
		
	void Renderer::SwapTriangleVertices(Vector* p1, Vector* p2, double* l1, double* l2)
	{
		Vector temp = *p1;
		double templ = *l1;
		
		*p1 = *p2;
		*p2 = temp;
		*l1 = *l2;
		*l2 = templ;
	}

	void Renderer::SortTriangleVertices(Vector* p1, Vector* p2, Vector* p3, double* l1, double* l2, double* l3)
	{
		if (p1->y < p2->y)
		{
			if (p3->y < p1->y)
				SwapTriangleVertices(p1, p3, l1, l3);
		}
		else
		{
			if (p2->y < p3->y)
				SwapTriangleVertices(p1, p2, l1, l2);
			else 
				SwapTriangleVertices(p1, p3, l1, l3);	
		}
		if (p2->y > p3->y)
			SwapTriangleVertices(p2, p3, l2, l3);
		
	}

	void Renderer::ClipPlane(Vertex vertices[], int vertices_c, Vertex* result, int* result_c, int axis, bool sign)
	{
		Vertex v1 = vertices[vertices_c - 1];
		double v1val = sign ? v1.GetCoord(axis) : -v1.GetCoord(axis);
		bool v1inside = v1val <= v1.GetPosition()->w;
				
		for (int i = 0; i < vertices_c; i++)
		{
			Vertex v2 = vertices[i];
			double v2val = sign ? v2.GetCoord(axis) : -v2.GetCoord(axis);
			bool v2inside = v2val <= v2.GetPosition()->w;

			if ((!v1inside && v2inside) || (v1inside && !v2inside))
			{
				double val = Render::GetLerpValueToPosition(v1.GetPosition()->w, v2.GetPosition()->w, v1val, v2val);

				result[(*result_c)++] = v1.LerpPosition(&v2, val);
			}

			if (v2inside)			
				result[(*result_c)++] = v2;			

			v1 = v2;
			v1val = v2val;
			v1inside = v2inside;
		}
	}

	bool Renderer::ClipX(Vertex vertices[], int* vertices_c, Vertex axisverts[])
	{			
		int axisverts_c = 0;
		ClipPlane(vertices, *vertices_c, axisverts, &axisverts_c, 0, false);

		if (axisverts_c == 0)
		{
			*vertices_c = 0;
			return false;
		}

		*vertices_c = 0;
		ClipPlane(axisverts, axisverts_c, vertices, vertices_c, 0, true);
		
		return vertices_c != 0;
	}

	bool Renderer::ClipY(Vertex vertices[], int* vertices_c, Vertex axisverts[])
	{
		int axisverts_c = 0;
		ClipPlane(vertices, *vertices_c, axisverts, &axisverts_c, 1, false);

		if (axisverts_c == 0)
		{
			*vertices_c = 0;
			return false;
		}

		*vertices_c = 0;
		ClipPlane(axisverts, axisverts_c, vertices, vertices_c, 1, true);

		return vertices_c != 0;
	}

	bool Renderer::ClipZ(Vertex vertices[], int* vertices_c, Vertex axisverts[])
	{
		int axisverts_c = 0;
		ClipPlane(vertices, *vertices_c, axisverts, &axisverts_c, 2, false);

		if (axisverts_c == 0)
		{
			*vertices_c = 0;
			return false;
		}

		*vertices_c = 0;
		ClipPlane(axisverts, axisverts_c, vertices, vertices_c, 2, true);

		return vertices_c != 0;
	}

	bool Renderer::ClipVertices(Vertex vertices[], int* vertices_c)
	{
		Vertex axisverts[10];

		bool candraw = ClipX(vertices, vertices_c, axisverts) && ClipY(vertices, vertices_c, axisverts) && ClipZ(vertices, vertices_c, axisverts);

		return candraw;
	}

	void Renderer::DrawTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3, uint8_t color, bool lit, bool cull, bool model)
	{
		Vector p1 = *(v1->GetPosition());
		Vector p2 = *(v2->GetPosition());
		Vector p3 = *(v3->GetPosition());

		Vector edge1 = p2 - p1;
		Vector edge2 = p3 - p1;
		
#if FLAT_SHADING 
	
		Vector facenormal = edge1.GetCrossProduct(&edge2);
		facenormal.Normalize();
		facenormal.w = 0;
		
		NormalTransform(transform, &facenormal);
		facenormal.Normalize();

		v1->SetNormal(&facenormal);
		v2->SetNormal(&facenormal);
		v3->SetNormal(&facenormal);
#else
		v1->GetNormal()->w = 0;
		v2->GetNormal()->w = 0;
		v3->GetNormal()->w = 0;
		
		NormalTransform(transform, v1->GetNormal());
		NormalTransform(transform, v2->GetNormal());
		NormalTransform(transform, v3->GetNormal());
		
		v1->GetNormal()->Normalize();
		v2->GetNormal()->Normalize();
		v3->GetNormal()->Normalize();
#endif

		ModelTransform(transform, v1->GetPosition()); ViewTransform(transform, v1->GetPosition()); ProjectionTransform(transform, v1->GetPosition());
		ModelTransform(transform, v2->GetPosition()); ViewTransform(transform, v2->GetPosition()); ProjectionTransform(transform, v2->GetPosition());
		ModelTransform(transform, v3->GetPosition()); ViewTransform(transform, v3->GetPosition()); ProjectionTransform(transform, v3->GetPosition());

		if (Base::InFrustum(v1->GetPosition()) && Base::InFrustum(v2->GetPosition()) && Base::InFrustum(v3->GetPosition()))
		{
			DrawClippedTriangle(transform, v1, v2, v3, color, lit, cull, model);
			return;
		}
		
		int clippedverts_c = 0;

		Vertex clippedverts[10];
		clippedverts[clippedverts_c++] = *v1;
		clippedverts[clippedverts_c++] = *v2;
		clippedverts[clippedverts_c++] = *v3;
	
		if (ClipVertices(clippedverts, &clippedverts_c))
		{
			Vertex firstvert = clippedverts[0];

			for (int i = 1; i < clippedverts_c - 1; i++)			
				DrawClippedTriangle(transform, &firstvert, &clippedverts[i], &clippedverts[i + 1], color, lit, cull, model);			
		}
		
	}

	void Renderer::DrawClippedTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3, uint8_t color, bool lit, bool cull, bool model)
	{
		Vector p1 = *(v1->GetPosition());
		Vector p2 = *(v2->GetPosition());
		Vector p3 = *(v3->GetPosition());

		if (cull)
		{		
			Vector center = Vector((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3, (p1.z + p2.z + p3.z) / 3);

			Vector edge1 = p2 - p1;
			Vector edge2 = p3 - p1;

			Vector facenormal = model ? -edge1.GetCrossProduct(&edge2) : edge1.GetCrossProduct(&edge2);

			if (center.GetDotProduct(&facenormal) > 0)
				return;
		}

		pScreen->triangles = pScreen->triangles++;
		

		Vector cp1 = p1;
		Vector cp2 = p2;
		Vector cp3 = p3;

		//remove view + projection to get model lights
		IProjectionTransform(transform, &cp1); IViewTransform(transform, &cp1);
		IProjectionTransform(transform, &cp2); IViewTransform(transform, &cp2);
		IProjectionTransform(transform, &cp3); IViewTransform(transform, &cp3);
		
		double l1 = lit ? lightingmanager.CalculateAllLightAmount(&cp1, v1->GetNormal(), model) : 0;
		double l2 = lit ? lightingmanager.CalculateAllLightAmount(&cp2, v2->GetNormal(), model) : 0;
		double l3 = lit ? lightingmanager.CalculateAllLightAmount(&cp3, v3->GetNormal(), model) : 0;
		
		NDCTransform(&p1);
		NDCTransform(&p2);
		NDCTransform(&p3);

		SortTriangleVertices(&p1, &p2, &p3, &l1, &l2, &l3);	
		
		double area = MathUtil::TriangleArea(&p1, &p2, &p3);
	
		bool right = area >= 0;
			
		TriangleEdge topmid = TriangleEdge(&p1, l1, &p2, l2);
		TriangleEdge topbot = TriangleEdge(&p1, l1, &p3, l3);
		TriangleEdge midbot = TriangleEdge(&p2, l2, &p3, l3);

		
		if (right)
		{
			DrawEdges(&topbot, &topmid, right, color, lit, transform);
			DrawEdges(&topbot, &midbot, right, color, lit, transform);
		}								
		else							
		{								
			DrawEdges(&topmid, &topbot, right, color, lit, transform);
			DrawEdges(&midbot, &topbot, right, color, lit, transform);
		}
		
	}

	void Renderer::DrawEdges(TriangleEdge* leftedge, TriangleEdge* rightedge, bool right, uint8_t color, bool lit, Transform* transform)
	{
		int starty = right ? (int)ceil(rightedge->GetStartY()) : (int)ceil(leftedge->GetStartY());
		int endy = right ? (int)ceil(rightedge->GetEndY()) : (int)ceil(leftedge->GetEndY());
		
		for (int y = starty; y < endy; y++)
		{
		
			int leftx = (int)ceil(leftedge->GetCurrentX());
			int rightx = (int)ceil(rightedge->GetCurrentX());
				
			double leftz = leftedge->GetCurrentZ();
			double rightz = rightedge->GetCurrentZ();
			double leftw = leftedge->GetCurrentW();
			double rightw = rightedge->GetCurrentW();

			double leftl = leftedge->GetCurrentLight();
			double rightl = rightedge->GetCurrentLight();

			double zslope = MathUtil::CalculateSlope(rightz, leftz, rightx, leftx);
			double wslope = MathUtil::CalculateSlope(rightw, leftw, rightx, leftx);
			double lslope = MathUtil::CalculateSlope(rightl, leftl, rightx, leftx);

			double z = leftz;
			double w = leftw;
			double l = leftl;
						
			for (int x = leftx; x < rightx; x++)
			{
			
				if (lit)
				{	
					uint8_t intensity = (uint8_t)floor(MathUtil::Interpolate(l, 0, 1, 0, 15));

					for (int i = 0; i < lightingmanager.GetLightCount(); i++)
					{
						BaseLight* light = lightingmanager.GetLight(i);

						if (!light->CastsShadows())
							continue;

						int type = light->GetType();

						Vector point = Vector(x, y, z, w);
			
						INDCTransform(&point);
				
						IProjectionTransform(transform, &point);

						IViewTransform(transform, &point);

						LightTransform(light, &point);

						point.PerspectiveDivide();
					
						int shadowx = (int)MathUtil::Interpolate(point.x, -1, 1, 0, 200);
						int shadowy = (int)MathUtil::Interpolate(point.y, 1, -1, 0, 200);

						double cz = light->GetShadowMap()->GetDepthPoint(shadowx, shadowy);

						if (MathUtil::InRange(point.x, -1, 1) && MathUtil::InRange(point.y, -1, 1))
						{										
							if (!(type == 2 && !MathUtil::InRange(point.z, -1, 1)))
							{
								double bias = type == 0 ? 0.1 : 0.01;

								if ((cz + bias) < point.z)
									intensity = (uint8_t)(intensity * 0.2);
							}
						}

					}	
									
					#pragma omp critical	
						DrawPoint(&Vector(x, y, z), intensity);
				}
				else
				{
					#pragma omp critical					
						DrawPoint(&Vector(x, y, z), color);					
				}

				z += zslope;
				w += wslope;
				l += lslope;
				l = MathUtil::Clamp(l, 0, 1);
			}
			
			
			leftedge->Step();
			rightedge->Step();
		}

	}



	void Renderer::DrawShadowMapTriangle(Transform* transform, BaseLight* light, Vertex* v1, Vertex* v2, Vertex* v3)
	{
		ModelTransform(transform, v1->GetPosition());
		ModelTransform(transform, v2->GetPosition());
		ModelTransform(transform, v3->GetPosition());
				
		LightTransform(light, v1->GetPosition());
		LightTransform(light, v2->GetPosition());
		LightTransform(light, v3->GetPosition());		

		if (Base::InFrustum(v1->GetPosition()) && Base::InFrustum(v2->GetPosition()) && Base::InFrustum(v3->GetPosition()))
		{
			DrawClippedShadowMapTriangle(light, v1, v2, v3);
			return;
		}

		int clippedverts_c = 0;

		Vertex clippedverts[10];
		clippedverts[clippedverts_c++] = *v1;
		clippedverts[clippedverts_c++] = *v2;
		clippedverts[clippedverts_c++] = *v3;

		if (ClipVertices(clippedverts, &clippedverts_c))
		{
			Vertex firstvert = clippedverts[0];

			for (int i = 1; i < clippedverts_c - 1; i++)
				DrawClippedShadowMapTriangle(light, &firstvert, &clippedverts[i], &clippedverts[i + 1]);
		}
	}

	void Renderer::DrawClippedShadowMapTriangle(BaseLight* light, Vertex* v1, Vertex* v2, Vertex* v3)
	{
		Vector p1 = *(v1->GetPosition());
		Vector p2 = *(v2->GetPosition());
		Vector p3 = *(v3->GetPosition());

		Vector n1 = *(v1->GetNormal());
		Vector n2 = *(v2->GetNormal());
		Vector n3 = *(v3->GetNormal());

		Vector center = Vector((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3, (p1.z + p2.z + p3.z) / 3);

		Vector edge1 = p2 - p1;
		Vector edge2 = p3 - p1;

		Vector facenormal = edge1.GetCrossProduct(&edge2);

		//if (center.GetDotProduct(&facenormal) > 0)
			//return;
		
		LightNDCTransform(light, &p1);
		LightNDCTransform(light, &p2);
		LightNDCTransform(light, &p3);

		double l1 = 0, l2 = 0, l3 = 0;

		SortTriangleVertices(&p1, &p2, &p3, &l1, &l2, &l3);

		double area = MathUtil::TriangleArea(&p1, &p2, &p3);
		bool right = area >= 0;

		TriangleEdge topmid = TriangleEdge(&p1, l1, &p2, l2);
		TriangleEdge topbot = TriangleEdge(&p1, l1, &p3, l3);
		TriangleEdge midbot = TriangleEdge(&p2, l2, &p3, l3);

		if (right)
		{
			DrawShadowMapEdges(light, &topbot, &topmid, right);
			DrawShadowMapEdges(light, &topbot, &midbot, right);
		}
		else
		{
			DrawShadowMapEdges(light, &topmid, &topbot, right);
			DrawShadowMapEdges(light, &midbot, &topbot, right);
		}
	}

	void Renderer::DrawShadowMapEdges(BaseLight* light, TriangleEdge* leftedge, TriangleEdge* rightedge, bool right)
	{
		int starty = right ? (int)ceil(rightedge->GetStartY()) : (int)ceil(leftedge->GetStartY());
		int endy = right ? (int)ceil(rightedge->GetEndY()) : (int)ceil(leftedge->GetEndY());

		for (int y = starty; y < endy; y++)
		{
			int leftx = (int)ceil(leftedge->GetCurrentX());
			int rightx = (int)ceil(rightedge->GetCurrentX());

			double leftz = leftedge->GetCurrentZ();
			double rightz = rightedge->GetCurrentZ();

			double zslope = MathUtil::CalculateSlope(rightz, leftz, rightx, leftx);
	
			double z = leftz;

			for (int x = leftx; x < rightx; x++)
			{
		//		#pragma omp critical	

				double cz = light->GetShadowMap()->GetDepthPoint(x, y);

				if (cz < z)
					continue;
				
				//DrawPoint(&Vector(x, y, z), intensity);

				light->GetShadowMap()->SetDepthPoint(x, y, z);

				z += zslope;
			}

			leftedge->Step();
			rightedge->Step();
		}
	}
	
	void Renderer::NormalTransform(Transform* transform, Vector* norm)
	{
		Matrix transformationm = transform->GetTransformationMatrix();

		*norm = transformationm * *norm;
		norm->Normalize();
	}

	void Renderer::ModelTransform(Transform* transform, Vector* pos)
	{
		Matrix transformationm = transform->GetTransformationMatrix();

		*pos = transformationm * *pos;
	}

	void Renderer::IModelTransform(Transform* transform, Vector* pos)
	{
		Matrix transformationm = transform->GetTransformationMatrix();
		
		Matrix itransformationm = Matrix();
		itransformationm.SetIdentityMatrix();

		bool hasmat = transformationm.GetInverseMatrix(&itransformationm);
		
		*pos = itransformationm * *pos;
	}

	void Renderer::ViewTransform(Transform* transform, Vector* pos)
	{
		Matrix cameram = pCamera->GetViewMatrix();

		*pos = cameram * *pos;
	}

	void Renderer::IViewTransform(Transform* transform, Vector* pos)
	{
		Matrix cameram = pCamera->GetViewMatrix();

		Matrix icameram = Matrix();
		icameram.SetIdentityMatrix();

		bool hasmat = cameram.GetInverseMatrix(&icameram);

		*pos = icameram * *pos;
	}

	void Renderer::ProjectionTransform(Transform* transform, Vector* pos)
	{
		Matrix cameraprojm = pCamera->GetProjectionMatrix();

		*pos = cameraprojm * *pos;
	}

	void Renderer::IProjectionTransform(Transform* transform, Vector* pos)
	{
		Matrix cameraprojm = pCamera->GetProjectionMatrix();

		Matrix icameraprojm = Matrix();
		icameraprojm.SetIdentityMatrix();

		bool hasmat = cameraprojm.GetInverseMatrix(&icameraprojm);

		*pos = icameraprojm * *pos;
	}

	void Renderer::NDCTransform(Vector* pos)
	{
		Matrix viewport = screenspacem;
		*pos = viewport * *pos;

		pos->PerspectiveDivide();
	}

	void Renderer::INDCTransform(Vector* pos)
	{
		pos->PerspectiveMultiply();

		Matrix viewport = screenspacem;

		Matrix iviewport = Matrix();
		iviewport.SetIdentityMatrix();

		bool hasmat = viewport.GetInverseMatrix(&iviewport);

		*pos = iviewport * *pos;
	}

	void Renderer::LightTransform(BaseLight* light, Vector* pos)
	{
		Matrix lightmat = light->GetLightMatrix();

		*pos = lightmat * *pos;
	}

	void Renderer::LightNDCTransform(BaseLight* light, Vector* pos)
	{
		ShadowMap* shadowmap = light->GetShadowMap();

		Matrix viewport = Matrix();
		viewport.SetScreenSpaceMatrix(shadowmap->GetWidth(), shadowmap->GetHeight(), pScreen->GetFontWidth(), pScreen->GetFontHeight());
		//viewport.SetShadowMapBiasMatrix();
		*pos = viewport * *pos;

		pos->PerspectiveDivide();
	}

		
}


