#ifndef Renderer_HPP
#define Renderer_HPP

#include <stdint.h>

#include "Util.hpp"
#include "Screen.hpp"
#include "Vector.hpp"
#include "Vertex.hpp"
#include "Matrix.hpp"
#include "TriangleEdge.hpp"
#include "Camera.hpp"
#include "LightingManager.hpp"
#include "ShadowMap.hpp"
#include "BaseLight.hpp"

using namespace Math;
using namespace Lighting;

namespace Render
{
	class Renderer
	{

	private:

		Screen* pScreen;
		double* pDepthBuffer;

		Camera* pCamera;
		LightingManager lightingmanager;

		short width;
		short height;
			
		Matrix screenspacem;

		
		void SwapTriangleVertices(Vector* p1, Vector* p2, double* l1, double* l2);
		void SortTriangleVertices(Vector* p1, Vector* p2, Vector* p3, double* l1, double* l2, double* l3);	

		void ClipPlane(Vertex vertices[], int vertices_c, Vertex* result, int* result_c, int axis, bool sign);
		bool ClipX(Vertex vertices[], int* vertices_c, Vertex axisverts[]);
		bool ClipY(Vertex vertices[], int* vertices_c, Vertex axisverts[]);
		bool ClipZ(Vertex vertices[], int* vertices_c, Vertex axisverts[]);
		bool ClipVertices(Vertex vertices[], int* vertices_c);

		void DrawClippedTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3, uint8_t color, bool lit, bool cull);
		void DrawClippedShadowMapTriangle(BaseLight* light, Vertex* v1, Vertex* v2, Vertex* v3);

		void DrawEdges(TriangleEdge* leftedge, TriangleEdge* rightedge, bool right, uint8_t color, bool lit, Transform* transform);
		void DrawShadowMapEdges(BaseLight* light, TriangleEdge* leftedge, TriangleEdge* rightedge, bool right);

		void ModelTransform(Transform* transform, Vector* pos);
		void IModelTransform(Transform* transform, Vector* pos);
		void ViewTransform(Transform* transform, Vector* pos);
		void IViewTransform(Transform* transform, Vector* pos);
		void ProjectionTransform(Transform* transform, Vector* pos);
		void IProjectionTransform(Transform* transform, Vector* pos);
		void NDCTransform(Vector* pos);
		void INDCTransform(Vector* pos);

		void LightTransform(BaseLight* light, Vector* pos);
		void LightNDCTransform(BaseLight* light, Vector* pos);

		void NormalTransform(Transform* transform, Vector* norm);

	public:

		Renderer();
		Renderer(Screen* screen, double znear, double zfar);
		~Renderer();

		void ClearDepthBuffer();

		short ScrW();
		short ScrH();

		Camera* GetCamera();
		void SetCamera(Camera* cam);
		
		LightingManager* GetLightingManager();
		void SetLightingManager(LightingManager* lman);

		Screen* GetScreen();
		void SetScreen(Screen* screen);

		void DrawPoint(const Vector* point, uint8_t color);
		void DrawPoint(Transform* transform, Vertex* vert, uint8_t color);
			
		void DrawTriangle(Transform* transform, Vertex* v1, Vertex* v2, Vertex* v3, uint8_t color, bool lit, bool cull);
		
		void DrawShadowMapTriangle(Transform* transform, BaseLight* light, Vertex* v1, Vertex* v2, Vertex* v3);

	};
}

#endif