#include "Scene.hpp"

#include <math.h>
#include <stdint.h>
#include <Windows.h>

#include "Color.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Util.hpp"
#include "Generator.hpp"

using namespace Base;

namespace Game
{

	Scene::Scene(Screen* screen)
	{
		pRenderer = new Renderer(screen, 0.01, 1000);
		pCamera = new Camera(90);

		pRenderer->SetCamera(pCamera);

		Vertex* vertices = nullptr;
		uint32_t vertices_c = 0;
		uint32_t* indices = nullptr;
		uint32_t indices_c = 0;

		LoadModel(".\\monkey.obj", &vertices, &vertices_c, &indices, &indices_c);
		pMesh = new Mesh(vertices, vertices_c, indices, indices_c);

		GeneratePlane(&vertices, &vertices_c, &indices, &indices_c);
		pFloor = new Mesh(vertices, vertices_c, indices, 252);
		
		GenerateSphere(&vertices, &vertices_c, &indices, &indices_c, 2);
		pSphere = new Mesh(vertices, vertices_c, indices, indices_c);
		
		pCamera->MoveZ(-3);

		pSphere->SetTranslation(0, 0, -5);
		pFloor->SetTranslation(0, 0, -5);
		pFloor->SetScale(1, -1, 1);
		
		
		pMesh->RotatePitch(150);
		pMesh->SetScale(1, -1, 1);
		pMesh->SetTranslation(0, 0, -5);

		pFloor->CalculateNormals();
		pMesh->CalculateNormals();
		
		delete[] vertices;
		delete[] indices;

		light = PointLight(&Vector(0, 0, 0), 15, 2, 10);
		pRenderer->AddLight(&light);

		Vertex pointv[3] =
		{
			Vertex(-1, -1, 0),
			Vertex(0, 0, 0),
			Vertex(1, -1, 0)
		};
		uint32_t pointi[3] =
		{
			0, 1, 2
		};
		
		pPoint = new Mesh(pointv, 3, pointi, 3);
		pPoint->SetScale(0.05, 0.05, 0.05);

		long int cx = 0;
		long int cy = 0;
		FindCenter(&cx, &cy);
		SetCursorPos(cx, cy);
		ShowCursor(false);
	}
	
	Scene::~Scene()
	{
		delete pRenderer;
		delete pMesh;
		delete pCamera;
	}

	void Scene::Update(double updatedelta)
	{
		//pMesh->RotateRoll(0.05);
		//pMesh->RotatePitch(0.05);
		pMesh->RotateYaw(0.01);
		//pSphere->RotateYaw(0.05);

		if (GetKeyState(VK_CAPITAL))
		{
			long int cx = 0;
			long int cy = 0;
			long int mx = 0;
			long int my = 0;

			FindCenter(&cx, &cy);
			GetMousePosition(&mx, &my);

			int dmx = mx - cx;
			int dmy = my - cy;

			bool roty = dmx != 0;
			bool rotx = dmy != 0;

			if (roty)
				pCamera->RotateYaw(dmx * 0.1);
			if (rotx)
				pCamera->RotatePitch(dmy * 0.1);

			SetCursorPos(cx, cy);
		}
	}

	
	void Scene::Render(double renderdelta)
	{
		time += 0.1;

		light.SetPosition(&Vector(cos(time)* 2, 0, sin(time) * 2));
		pPoint->SetTranslation(&Vector(cos(time) * 2, 0, sin(time) * 2));

		pFloor->RenderMesh(pRenderer, true, 16);
		
		pMesh->RenderMesh(pRenderer, true, 16);
		//pSphere->RenderMesh(pRenderer, true, 16);
		pPoint->RenderMesh(pRenderer, true, 15);

		pRenderer->ClearDepthBuffer();
	}

	void Scene::Input(INPUT_RECORD* inputevent)
	{
		if (inputevent->EventType == KEY_EVENT)
		{
			switch (inputevent->Event.KeyEvent.wVirtualKeyCode)
			{

			case VK_LEFT:
				pCamera->RotateYaw(-5);
				break;

			case VK_RIGHT:
				pCamera->RotateYaw(5);
				break;

			case VK_UP:
				pCamera->RotatePitch(-5);
				break;

			case VK_DOWN:
				pCamera->RotatePitch(5);
				break;
			case VK_CONTROL:
				pCamera->MoveY(-0.5);
				break;

			case VK_SPACE:
				pCamera->MoveY(0.5);
				break;

			case 0x41 :
				pCamera->MoveX(-0.5);
				break;

			case 0x44:
				pCamera->MoveX(0.5);
				break;

			case 0x53:
				pCamera->MoveZ(-0.5);
				break;

			case 0x57:
				pCamera->MoveZ(0.5);
				break;

			}
		}
		else
		{

		}
	}
	
}