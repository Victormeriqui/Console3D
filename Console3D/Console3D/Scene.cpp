#include "Scene.hpp"

#include <math.h>

#include <Windows.h>

#include "Util.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Color.hpp"
#include "Generator.hpp"
#include "LightingManager.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"
#include "ShadowMap.hpp"

using namespace Base;

namespace Game
{
	Scene::Scene(Screen* screen)
	{
		pRenderer = new Renderer(screen, 0.1, 100);
		pCamera = pRenderer->GetCamera();
		pLightingManager = pRenderer->GetLightingManager();

		Vertex* vertices = nullptr;
		int vertices_c = 0;
		int* indices = nullptr;
		int indices_c = 0;

		LoadModel(".\\f1.obj", &vertices, &vertices_c, &indices, &indices_c);
		//GenerateSphere(&vertices, &vertices_c, &indices, &indices_c, 3);
		pMesh = new Mesh(vertices, vertices_c, indices, indices_c);
		pMesh->SetScale(1, 1, 1);
		//pMesh->SetScale(15, 15, 15);
		pMesh->SetTranslation(0, 0, 0);
		//pMesh->RotatePitch(90);
				
		//pCamera->RotateYaw(-110);

		pCamera->MoveZ(-3);		
		pCamera->MoveY(1);
		pCamera->RotatePitch(20);
		pMesh->CalculateNormals();

		GeneratePlane(&vertices, &vertices_c, &indices, &indices_c, 105, 105);
		pFloor = new Mesh(vertices, vertices_c, indices, indices_c);
		pFloor->SetTranslation(0, -2, 0);
		pFloor->SetScale(1, 1, 1);
		pFloor->CalculateNormals();

		GenerateSphere(&vertices, &vertices_c, &indices, &indices_c, 0);
		pRefMesh = new Mesh(vertices, vertices_c, indices, indices_c);
		pRefMesh->SetScale(0.1, 0.1, 0.1);

		delete[] vertices;
		delete[] indices;
		
		pLightingManager->SetAmbientLight(0.1);		
		//pLightingManager->AddDirectionalLight(new DirectionalLight(&Vector(0, -1, 1), 0.7, true));
		pLightingManager->AddPointLight(new PointLight(&Vector(0, 0, 0), 7, 100, false));
		//pLightingManager->AddSpotLight(new SpotLight(&pCamera->GetPosition(), &pCamera->GetLookDirection(), 10, 100, 45, true));
			
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
		delete pRefMesh;
	}

	void Scene::Update(double updatedelta)
	{	
		//pMesh->Rotate(&Quaternion(&Vector(0, 1, 0), 0.001));	
	}

	
	void Scene::Render(double renderdelta)
	{
		time += 0.05;
		
		if (pLightingManager->GetPointLight(0) != nullptr)
		{
			PointLight* light = pLightingManager->GetPointLight(0);
			light->SetPosition(&pCamera->GetPosition());

			//pLightingManager->GetPointLight(0)->SetPosition(&Vector(cos(time)*3, 0, sin(time) * 3));				
			pRefMesh->SetTranslation(&pCamera->GetPosition());
		}
			
		if (pLightingManager->GetDirectionalLight(0) != nullptr)
		{
			DirectionalLight* light = pLightingManager->GetDirectionalLight(0);

			Vector dir = pCamera->GetLookDirection();

//			light->SetDirection(&dir);

		/*	for (int i = 0; i < 50; i++)
			{
				for (int i2 = 0; i2 < 50; i2++)
				{
					int shadx = MathUtil::Interpolate(i2, 0, 50, 0, 200);
					int shady = MathUtil::Interpolate(i, 0, 50, 0, 200);
					pRenderer->DrawPoint(&Vector(i2, i, -1), MathUtil::Interpolate(light->GetShadowMap()->GetDepthPoint(shadx, shady), -10, 10, 0, 15));
				}
			}*/

		}

		if (pLightingManager->GetSpotLight(0) != nullptr)
		{
			SpotLight* light = pLightingManager->GetSpotLight(0);
			
			if ((GetKeyState(VK_TAB) & 0x8000))
			{
				light->SetPosition(&pCamera->GetPosition());
				Vector dir = pCamera->GetLookDirection();

				light->SetDirection(&dir);
				pRefMesh->SetTranslation(&pCamera->GetPosition());
			}		
		}
		
		if (shadows)
		{			
			for (int i = 0; i < pLightingManager->GetLightCount(); i++)
			{
				BaseLight* light = pLightingManager->GetLight(i);
				
				if (!light->CastsShadows())
					continue;
				
				light->GetShadowMap()->ClearDepthMap();
				pMesh->RenderShadowMap(pRenderer, light);
				
				if(light->GetType() != 0)
					pFloor->RenderShadowMap(pRenderer, light);
				
			}
		}

		pMesh->RenderLitMesh(pRenderer, true);
		pFloor->RenderLitMesh(pRenderer, true);
		
		//pMesh->RenderAutoMesh(pRenderer, true);
	//	pFloor->RenderAutoMesh(pRenderer, true);		
	
		//pMesh->RenderColoredMesh(pRenderer, 10, true);
		//pFloor->RenderColoredMesh(pRenderer, 10, true);

		pRefMesh->RenderColoredMesh(pRenderer, 15, true);

		pRenderer->ClearDepthBuffer();
	
	}

	void Scene::Input()
	{
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

		double movespeed = 0.001;
		
		if ((GetKeyState(VK_LSHIFT) & 0x8000))
			movespeed = 0.0001;

		if ((GetKeyState(VK_CONTROL) & 0x8000))
			movespeed = 0.0100;

		if ((GetKeyState(VK_LEFT) & 0x8000))
			pCamera->RotateYaw(-movespeed);
			
		if ((GetKeyState(VK_RIGHT) & 0x8000))
			pCamera->RotateYaw(movespeed);
		
		if ((GetKeyState(VK_UP) & 0x8000))
			pCamera->RotatePitch(-movespeed);
			
		if ((GetKeyState(VK_DOWN) & 0x8000))
			pCamera->RotatePitch(movespeed);
				
		if ((GetKeyState(VK_SPACE) & 0x8000))
			pCamera->MoveY(movespeed);
							
		if ((GetKeyState(0x41) & 0x8000))
			pCamera->MoveX(-movespeed);
			
		if ((GetKeyState(0x44) & 0x8000))
			pCamera->MoveX(movespeed);
			
		if ((GetKeyState(0x53) & 0x8000))
			pCamera->MoveZ(-movespeed);
			
		if ((GetKeyState(0x57) & 0x8000))
			pCamera->MoveZ(movespeed);
	}
	
}