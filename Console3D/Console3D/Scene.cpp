#include "Scene.hpp"

#include <math.h>

#include <Windows.h>

#include "Util.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
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

		model = new Model(".\\Models\\md2\\scarlet.md2");
		model->SetTranslation(3.5, 1, 3.5);
		model->SetScale(0.05, 0.05, 0.05);
		model->RotatePitch(-90);

		LoadModel(".\\Models\\obj\\gourd.obj", &vertices, &vertices_c, &indices, &indices_c);

		//GenerateSphere(&vertices, &vertices_c, &indices, &indices_c, 3);
		pMesh = new Mesh(vertices, vertices_c, indices, indices_c);
		//pMesh->SetScale(0.05, 0.05, 0.05);
		pMesh->SetScale(1, 1, 1);
		pMesh->SetTranslation(0, 0, 0);
		pMesh->CalculateNormals();
		//pMesh->RotatePitch(90);
				
		//pCamera->RotateYaw(-110);

		pCamera->MoveZ(-3);		
		pCamera->MoveY(1);
		pCamera->RotatePitch(20);
	

		GeneratePlane(&vertices, &vertices_c, &indices, &indices_c, 30, 30, 4);
		pFloor = new Mesh(vertices, vertices_c, indices, indices_c);
		pFloor->SetTranslation(0, 0, 0);
		pFloor->RotatePitch(90);
		pFloor->SetScale(1, 1, 1);
		pFloor->CalculateNormals();

		GenerateSphere(&vertices, &vertices_c, &indices, &indices_c, 0);
		pRefMesh = new Mesh(vertices, vertices_c, indices, indices_c);
		pRefMesh->SetScale(0.1, 0.1, 0.1);

		delete[] vertices;
		delete[] indices;
		
		pLightingManager->SetAmbientLight(0.1);		
		pLightingManager->AddDirectionalLight(new DirectionalLight(&Vector(0, -1, 1), 0.3, false));
		//pLightingManager->AddPointLight(new PointLight(&Vector(0, 0, 0), 10, 100, false));
		//pLightingManager->AddSpotLight(new SpotLight(&Vector(0, 2, 0), &(pCamera->GetLookDirection()+Vector(0.1, -1, 0)), 10, 100, 45, false));
			
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
		//pMesh->Rotate(&Quaternion(&Vector(0, 1, 0), 0.0001));	
		/*pMesh->SetScale(&Vector(2+sin(time), 1, 1));
		
		Vertex* vertices = pMesh->GetVertices();
		int verts_c = pMesh->GetVerticesCount();

		for (int i = 0; i < verts_c; i++)
		{
			if (i % 5 == 0)
			{
		//		Vector* pos = vertices[i].GetPosition();
			//	pos->z = sin(time);
			}
		}*/
	}

	
	void Scene::Render(double renderdelta)
	{
		time += 0.05;
	
		if (pLightingManager->GetPointLight(0) != nullptr)
		{
			PointLight* light = pLightingManager->GetPointLight(0);
			light->SetPosition(&(pCamera->GetPosition() + Vector(0, -1, 0)));

			//pLightingManager->GetPointLight(0)->SetPosition(&Vector(cos(time)*3, 0, sin(time) * 3));				
			pRefMesh->SetTranslation(&pCamera->GetPosition());
		}
		
		if (pLightingManager->GetDirectionalLight(0) != nullptr)
		{
			DirectionalLight* light = pLightingManager->GetDirectionalLight(0);

			Vector dir = pCamera->GetLookDirection();
			if ((GetKeyState(VK_TAB) & 0x8000))
			{
				light->SetDirection(&dir);
				pRefMesh->SetTranslation(&pCamera->GetPosition());
			}
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
			if ((GetKeyState(VK_INSERT) & 0x8000))
			{
				light->SetCastsShadows(true);
				light->SetIntensity(10);
			}
			if ((GetKeyState(VK_DELETE) & 0x8000))
			{
				light->SetCastsShadows(false);
				light->SetIntensity(0);
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
				model->RenderShadowMap(pRenderer, light);

				if(light->GetType() != 0)
					pFloor->RenderShadowMap(pRenderer, light);
				
			}
		}

		//pMesh->RenderLitMesh(pRenderer, true);
		
		model->RenderLitModel(pRenderer, true);
		
		pFloor->RenderLitMesh(pRenderer, true);
		
		//pMesh->RenderAutoMesh(pRenderer, true);
	//	pFloor->RenderAutoMesh(pRenderer, true);		
	
	//	pMesh->RenderColoredMesh(pRenderer, 10, false);
		//pFloor->RenderColoredMesh(pRenderer, 4, true);

		//pRefMesh->RenderColoredMesh(pRenderer, 15, true);

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
		
		if ((GetKeyState(0x46) & 0x8000))
		{
			Animation salute;
			for (int i = 0; i < 10; i++)
				salute.frames[i] = 84 + i;
			salute.frames_c = 10;

			model->PlayAnimation(&salute);
		}
		if ((GetKeyState(0x47) & 0x8000))
		{
			Animation taunt;
		//	for (int i = 0; i < 16; i++)
			//	taunt.frames[i] = 95 + i;
			taunt.frames[0] = 95;
			taunt.frames[1] = 96;
			taunt.frames[2] = 97;
			taunt.frames[3] = 98;
			taunt.frames[4] = 99;
			taunt.frames[5] = 100;
			taunt.frames[6] = 101;
			taunt.frames[7] = 102;
			taunt.frames[8] = 103;
			taunt.frames[9] = 104;
			taunt.frames[10] = 105;
			taunt.frames[11] = 106;
			taunt.frames[12] = 107;
			taunt.frames[13] = 108;
			taunt.frames[14] = 109;
			taunt.frames[15] = 100;
			taunt.frames[16] = 101;
			taunt.frames[17] = 102;
			taunt.frames[18] = 103;
			taunt.frames[19] = 104;
			taunt.frames[20] = 105;
			taunt.frames[21] = 106;
			taunt.frames[22] = 107;
			taunt.frames[23] = 108;
			taunt.frames[24] = 109;
			taunt.frames[25] = 0;
			
			//taunt.frames[15] = 110;
		//	taunt.frames[16] = 111;
		//	taunt.frames[17] = 0;
			taunt.frames_c = 26;

			model->PlayAnimation(&taunt);
		}
		if ((GetKeyState(0x48) & 0x8000))
		{
			Animation stand;
			for (int i = 0; i < 6; i++)
				stand.frames[i] = 40+i;

			stand.frames[6] = 45;
			stand.frames[7] = 44;
			stand.frames[8] = 43;
			stand.frames[9] = 42;
			stand.frames[10] = 41;
			stand.frames[11] = 40;
			stand.frames_c = 12;

			model->PlayAnimation(&stand);
		}
		if ((GetKeyState(0x49) & 0x8000))
		{
			Animation skel;
			for (int i = 0; i < 33; i++)
				skel.frames[i] = 84 + i;
			skel.frames_c = 33;

			model->PlayAnimation(&skel);
		}


		if ((GetKeyState(VK_LSHIFT) & 0x8000))
			movespeed = 0.0001;

		if ((GetKeyState(VK_CONTROL) & 0x8000))
			movespeed = 0.0100;

		if ((GetKeyState(VK_LEFT) & 0x8000))
			pCamera->RotateYaw(-0.02);
			
		if ((GetKeyState(VK_RIGHT) & 0x8000))
			pCamera->RotateYaw(0.02);
		
		if ((GetKeyState(VK_UP) & 0x8000))
			pCamera->RotatePitch(-0.01);
			
		if ((GetKeyState(VK_DOWN) & 0x8000))
			pCamera->RotatePitch(0.01);
				
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