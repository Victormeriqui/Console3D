#ifndef Renderer_HPP
#define Renderer_HPP

#include <stdint.h>

#include "Screen.hpp"
#include "Vector.hpp"
#include "Vertex.hpp"
#include "Matrix.hpp"
#include "TriangleEdge.hpp"
#include "Camera.hpp"
#include "PointLight.hpp"

using namespace Math;

namespace Render
{
	class Renderer
	{

	private:

		Screen* pScreen;
		double* pDepthBuffer;

		Camera* pCamera;

		uint16_t width;
		uint16_t height;
		
		Matrix pixelratiom;
		Matrix screenspacem;
		Matrix projectionm;
		double znear;
		double zfar;

		PointLight* lights[5];
		uint16_t lights_c;

		void SwapTriangleVertices(Vector* p1, Vector* p2, double* l1, double* l2);
		void SortTriangleVertices(Vector* p1, Vector* p2, Vector* p3, double* l1, double* l2, double* l3);
	
		void DrawEdges(TriangleEdge* leftedge, TriangleEdge* rightedge, bool right, uint16_t color);

		void SaturateLight(double* light);

	public:

		Renderer();
		Renderer(Screen* screen, double znear, double zfar);
		~Renderer();

		void ClearDepthBuffer();

		uint16_t ScrW();
		uint16_t ScrH();

		Camera* GetCamera();
		void SetCamera(Camera* cam);
		
		Screen* GetScreen();
		void SetScreen(Screen* screen);

		double GetZNear();
		double GetZFar();

		void SetZNear(double val);
		void SetZFar(double val);

		void DrawPoint(uint16_t x, uint16_t y, double z, uint16_t color);
		void DrawPoint(Transform* transform, Vertex* vert, uint16_t color);

		void DrawLine(uint16_t sx, uint16_t sy, double sz, uint16_t ex, uint16_t ey, double ez, uint16_t color);
		void DrawLine(Transform* transform, Vertex* v1, Vertex* v2, uint16_t color);
		
		void DrawLineTriangle(uint16_t x1, uint16_t y1, double z1, uint16_t x2, uint16_t y2, double z2, uint16_t x3, uint16_t y3, double z3, uint16_t color);
		void DrawLineTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3, uint16_t color);

		void DrawTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3, uint16_t color);
		void DrawLightningTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3);

		void PositionTransform(Transform* transform, Vector* pos);
		void NormalTransform(Transform* transform, Vector* norm);

		double CalculatePointLights(const Vector* pos, const Vector* normal);

		void AddLight(PointLight* light);
		void RemoveLight(PointLight* light);
	};
}

#endif