#ifndef MODEL_HPP
#define MODEL_HPP

#include "Renderer.hpp"
#include "Vertex.hpp"
#include "Transform.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"
#include "MD2.hpp"

namespace Render
{
	typedef struct
	{
		int frames[200];
		int frames_c;
	} Animation;

	class Model
	{

	private:
		
		Vertex* pVertices;
		Vertex** pFrames;
		int frames_c;
		int vertices_c;
		int* pIndices;
		int indices_c;
		MD2_Triangle* triangles;
		int triangles_c;

		Animation curanim;
		bool animating = false; 

		int framesize;
		double fps = 0.2;

		double curtime;
		double oldtime;
		
		int animframe;
		int curframe;
		int nextframe;
		double interpprog = 0;
	
		Transform transform;
	
	public:

		Model();
		Model(const char* filepath);
		~Model();

		void PlayAnimation(Animation* anim);
		void AddFrame(Vertex* vertices);
		void SetIndicies(int indices[], int indicescount);
		void CalculateNormals();

		Vertex** GetFrames();
		int GetFrameCount();
		int GetVertexCount();
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

		void RenderLitModel(Renderer* renderer, bool cull);
		void RenderShadowMap(Renderer* renderer, BaseLight* light);

	};

}


#endif
