#ifndef SCENE_HPP
#define SCENE_HPP

#include "Renderer.hpp"
#include "Screen.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "PointLight.hpp"

using namespace Render;

namespace Game
{
	class Scene
	{

	private:

		Renderer* pRenderer;
		Camera* pCamera;
		double time = 0;

		Mesh* pFloor;
		Mesh* pMesh;
		Mesh* pSphere;

		PointLight light;
		Mesh* pPoint;

	public:

		Scene(Screen* screen);
		~Scene();

		void Render(double renderdelta);
		void Update(double updatedelta);
		void Input(INPUT_RECORD* inputevent);

	};
}

#endif