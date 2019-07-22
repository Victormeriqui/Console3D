#ifndef SCENE_HPP
#define SCENE_HPP

#include "Renderer.hpp"
#include "Screen.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "LightingManager.hpp"
#include "PointLight.hpp"
#include "Model.hpp"

using namespace Render;
using namespace Lighting;

namespace Game
{

	class Scene
	{
		
	private:

		Renderer* pRenderer;
		Camera* pCamera;
		LightingManager* pLightingManager;
		
		bool shadows = true;
		double time = 0;

		Mesh* pFloor;
		Mesh* pMesh;
		Mesh* pRefMesh;
		Model* model;

	public:
		

		Scene(Screen* screen);
		~Scene();

		void Render(double renderdelta);
		void Update(double updatedelta);
		void Input();

	};
}

#endif