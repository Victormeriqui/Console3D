#include "Renderer.hpp"

#include <limits.h>
#include <stdlib.h>
#include <math.h>

#include "Vertex.hpp"
#include "Color.hpp"
#include "Matrix.hpp"
#include "TriangleEdge.hpp"

namespace Render
{

	Renderer::Renderer()
	{
	}

	Renderer::Renderer(Screen* screen, double znear, double zfar) :
		pScreen(screen), znear(znear), zfar(zfar)
	{
		width = screen->GetWidth();
		height = screen->GetHeight();

		pDepthBuffer = new double[width*height];
		ClearDepthBuffer();

		pCamera = nullptr;
		screenspacem = *Matrix().SetScreenSpaceMatrix(width, height);
		pixelratiom = *Matrix().SetPixelRatioMatrix(4, 3);

		lights_c = 0;
	}

	Renderer::~Renderer()
	{
		delete[] pDepthBuffer;
	}

	uint16_t Renderer::ScrW()
	{
		return width;
	}

	uint16_t Renderer::ScrH()
	{
		return height;
	}
	
	Camera* Renderer::GetCamera()
	{
		return pCamera;
	}

	void Renderer::SetCamera(Camera* cam)
	{
		pCamera = cam;
		projectionm = *Matrix().SetProjectionMatrix(width, height, znear, zfar, cam->GetFOV());
	}

	Screen* Renderer::GetScreen()
	{
		return pScreen;
	}

	void Renderer::SetScreen(Screen* screen)
	{
		pScreen = screen;
	}

	double Renderer::GetZNear()
	{
		return znear;
	}

	double Renderer::GetZFar()
	{
		return zfar;
	}

	void Renderer::SetZNear(double val)
	{
		znear = val;
	}

	void Renderer::SetZFar(double val)
	{
		zfar = val;
	}

	void Renderer::ClearDepthBuffer()
	{
		for (uint32_t i = 0; i < (uint16_t)(width*height); i++)
			pDepthBuffer[i] = INT_MIN;
	}

	void Renderer::DrawPoint(uint16_t x, uint16_t y, double z, uint16_t color)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return;

		double cz = pDepthBuffer[x + y * width];
		if (cz > z)
			return;

     	pDepthBuffer[x + y * width] = z;

		pScreen->SetPixel(x, y, color);
	}

	void Renderer::DrawPoint(Transform* transform, Vertex* vert, uint16_t color)
	{
		Vector pos = *(vert->GetPosition());

		PositionTransform(transform, &pos);

		DrawPoint((uint16_t)pos.x, (uint16_t)pos.y, (uint16_t)pos.z, color);
	}
	
	void Renderer::DrawLine(uint16_t x1, uint16_t y1, double z1, uint16_t x2, uint16_t y2, double z2, uint16_t color)
	{
		int xdist = x2 - x1;
		int ydist = y2 - y1;
		double zdist = z2 - z1;

		int modx = xdist > 0 ? 1 : -1;
		int mody = ydist > 0 ? 1 : -1;

		double slopey = ydist != 0 ? xdist / ydist : 0;

		uint16_t x = x1;
		uint16_t y = y1;
		double z = z1;

		if (abs(slopey) >= 1 || slopey == 0)
		{
			for (int sx = (int)x1; sx != (int)x2; sx += modx)
			{
				y = (uint16_t)(y1 + ydist * (sx - x1) / xdist);
				z = (z1 + zdist * (sx - x1) / xdist);
				DrawPoint(sx, y, z, color);
			}
		}
		else
		{
			for (int sy = (int)y1; sy != (int)y2; sy += mody)
			{
				x = (uint16_t)(x1 + xdist * (sy - y1) / ydist);
				z = (z1 + zdist * (sy - y1) / ydist);
				DrawPoint(x, sy, z, color);
			}
		}
		if (slopey == 0)
		{
			for (int sy = (int)y1; sy != (int)y2; sy += mody)
			{
				x = (uint16_t)(x1 + xdist * (sy - y1) / ydist);
				z = (z1 + zdist * (sy - y1) / ydist);
				DrawPoint(x, sy, z, color);
			}
		}
	}

	void Renderer::DrawLine(Transform* transform, Vertex* v1, Vertex* v2, uint16_t color)
	{
		Vector p1 = *(v1->GetPosition());
		Vector p2 = *(v2->GetPosition());

		PositionTransform(transform, &p1);
		PositionTransform(transform, &p2);
	
		DrawLine((uint16_t)p1.x, (uint16_t)p1.y, p1.z, (uint16_t)p2.x, (uint16_t)p2.y, p2.z, color);
	}

	void Renderer::DrawLineTriangle(uint16_t x1, uint16_t y1, double z1, uint16_t x2, uint16_t y2, double z2, uint16_t x3, uint16_t y3, double z3, uint16_t color)
	{
		DrawLine(x1, y1, z1, x2, y2, z2, color);
		DrawLine(x2, y2, z2, x3, y3, z3, color);
		DrawLine(x3, y3, z3, x1, y1, z1, color);
	}

	void Renderer::DrawLineTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3, uint16_t color)
	{
		Vector p1 = *(v1->GetPosition());
		Vector p2 = *(v2->GetPosition());
		Vector p3 = *(v3->GetPosition());

		PositionTransform(transform, &p1);
		PositionTransform(transform, &p2);
		PositionTransform(transform, &p3);

		DrawLineTriangle((uint16_t)p1.x, (uint16_t)p1.y, p1.z, (uint16_t)p2.x, (uint16_t)p2.y, p2.z, (uint16_t)p3.x, (uint16_t)p3.y, p3.z, color);
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


	void Renderer::DrawTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3, uint16_t color)
	{

		Vector p1 = *(v1->GetPosition());
		Vector p2 = *(v2->GetPosition());
		Vector p3 = *(v3->GetPosition());

		Vector n1 = *(v1->GetNormal());
		Vector n2 = *(v2->GetNormal());
		Vector n3 = *(v3->GetNormal());
				
		double l1 = 0;
		double l2 = 0;
		double l3 = 0;
		
		PositionTransform(transform, &p1);
		PositionTransform(transform, &p2);
		PositionTransform(transform, &p3);

		NormalTransform(transform, &n1);
		NormalTransform(transform, &n2);
		NormalTransform(transform, &n3);
		
		SortTriangleVertices(&p1, &p2, &p3, &l1, &l2, &l3);	
		
		double area = MathUtil::TriangleArea(&p1, &p2, &p3);
		bool right = area >= 0;
		

		TriangleEdge topmid = TriangleEdge(&p1, l1, &p2, l2);
		TriangleEdge topbot = TriangleEdge(&p1, l1, &p3, l3);
		TriangleEdge midbot = TriangleEdge(&p2, l2, &p3, l3);


		if (right)
		{
			DrawEdges(&topbot, &topmid, right, color);
			DrawEdges(&topbot, &midbot, right, color);
		}								
		else							
		{								
			DrawEdges(&topmid, &topbot, right, color);
			DrawEdges(&midbot, &topbot, right, color);
		}
		

		//DrawPoint(p1.x, p1.y, 100, BG_BLACK | FG_RED);
		//DrawPoint(p2.x, p2.y, 100, BG_BLACK | FG_GREEN);
		//DrawPoint(p3.x, p3.y, 100, BG_BLACK | FG_BLUE);

	}

	void Renderer::DrawLightningTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3)
	{

		Vector p1 = *(v1->GetPosition());
		Vector p2 = *(v2->GetPosition());
		Vector p3 = *(v3->GetPosition());

		Vector n1 = *(v1->GetNormal());
		Vector n2 = *(v2->GetNormal());
		Vector n3 = *(v3->GetNormal());

		double l1 = CalculatePointLights(&p1, &n1);
		double l2 = CalculatePointLights(&p2, &n2);
		double l3 = CalculatePointLights(&p3, &n3);

		PositionTransform(transform, &p1);
		PositionTransform(transform, &p2);
		PositionTransform(transform, &p3);

		NormalTransform(transform, &n1);
		NormalTransform(transform, &n2);
		NormalTransform(transform, &n3);

		SortTriangleVertices(&p1, &p2, &p3, &l1, &l2, &l3);

		double area = MathUtil::TriangleArea(&p1, &p2, &p3);
		bool right = area >= 0;


		TriangleEdge topmid = TriangleEdge(&p1, l1, &p2, l2);
		TriangleEdge topbot = TriangleEdge(&p1, l1, &p3, l3);
		TriangleEdge midbot = TriangleEdge(&p2, l2, &p3, l3);


		if (right)
		{
			DrawEdges(&topbot, &topmid, right, 16);
			DrawEdges(&topbot, &midbot, right, 16);
		}
		else
		{
			DrawEdges(&topmid, &topbot, right, 16);
			DrawEdges(&midbot, &topbot, right, 16);
		}


		//DrawPoint(p1.x, p1.y, 100, BG_BLACK | FG_RED);
		//DrawPoint(p2.x, p2.y, 100, BG_BLACK | FG_GREEN);
		//DrawPoint(p3.x, p3.y, 100, BG_BLACK | FG_BLUE);

	}

	void Renderer::DrawEdges(TriangleEdge* leftedge, TriangleEdge* rightedge, bool right, uint16_t color)
	{
		uint16_t starty = right ? (uint16_t)ceil(rightedge->GetStartY()) : (uint16_t)ceil(leftedge->GetStartY());
		uint16_t endy = right ? (uint16_t)ceil(rightedge->GetEndY()) : (uint16_t)ceil(leftedge->GetEndY());

		for (uint16_t y = starty; y < endy; y++)
		{
			uint16_t leftx = (uint16_t)ceil(leftedge->GetCurrentX());
			uint16_t rightx = (uint16_t)ceil(rightedge->GetCurrentX());
			
			double leftz = leftedge->GetCurrentZ();
			double rightz = rightedge->GetCurrentZ();
			
			double leftl = leftedge->GetCurrentLight();
			double rightl = rightedge->GetCurrentLight();

			double zslope = MathUtil::CalculateSlope(rightz, leftz, rightx, leftx);
			double lslope = MathUtil::CalculateSlope(rightl, leftl, rightx, leftx);

			double z = leftz;
			double l = leftl;

			for (uint16_t x = leftx; x <= rightx; x++)
			{
				uint16_t lcol = (uint16_t)ceil(MathUtil::Interpolate(l, 0, 1, 1, 15));

				DrawPoint(x, y, z, color == 16 ? lcol : color);

				z += zslope;
				l += lslope;
				l = MathUtil::Clamp(l, 0, 1);
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

	void Renderer::PositionTransform(Transform* transform, Vector* pos)
	{
		Matrix cameram = pCamera->GetCameraMatrix();
		Matrix transformationm = transform->GetTransformationMatrix();

		*pos = (screenspacem * (projectionm * pixelratiom * (cameram * transformationm))) * *pos;

		pos->PerspectiveDivide();
	}

	void Renderer::SaturateLight(double* light)
	{
		*light = MathUtil::Clamp(*light, 0, 1);
	}

	double Renderer::CalculatePointLights(const Vector* pos, const Vector* normal)
	{
		Vector vertpos = *pos;
		Vector vertnorm = *normal;

		double light = 0;
		for (uint16_t i = 0; i < lights_c; i++)
		{
			Vector lightdir = (lights[i]->GetPosition() - vertpos);
			double lightdist = lightdir.GetLength();
			
			if (lightdist >= lights[i]->GetRange())
				continue;

			lightdir.Normalize();
			Attenuation att = lights[0]->GetAttenuation();
			double attenuation = att.c + att.b * lightdist + att.a * lightdist * lightdist + 0.0001;

			double dirlight = vertnorm.GetDotProduct(&lightdir);

			light += dirlight * lights[i]->GetIntensity() / attenuation;
		}
		
		SaturateLight(&light);
		return light;
	}

	void Renderer::AddLight(PointLight* light)
	{
		lights[lights_c++] = light;
	}

	void Renderer::RemoveLight(PointLight* light)
	{
		for (uint16_t i = 0; i < lights_c; i++)
		{
			if (lights[i] == light)
			{
				for (uint16_t j = i; j < lights_c; j++)
					lights[j] = lights[j + 1];
				
				lights_c--;
				break;
			}
		}
	}

}


