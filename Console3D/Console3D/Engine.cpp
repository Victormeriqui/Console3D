#include "Engine.hpp"

#include <stdlib.h>

#include "Time.hpp"
#include "Color.hpp"

using namespace Render;

#define FRAMECAP 5000

namespace Base
{

	Engine::Engine(Screen* screen, Scene* scene, HANDLE* hOut) :
		pScreen(screen), pScene(scene), pHout(hOut)
	{
		running = false;
	}

	Engine::~Engine()
	{
		CleanUp();
	}

	void Engine::Start()
	{
		if (running)
			return;

		Run();
	}

	void Engine::Stop()
	{
		if (running)
			running = false;
	}

	void Engine::Run()
	{
		running = true;

		double updatetime = 1/5000.0;

		double simtime = 0;
		
		double curtime = Time::GetCurTime();
		double lasttime = curtime;

		double framesectime = 0;
		int frames = 0;

		bool shouldrender = false;

		while (running)
		{
			curtime = Time::GetCurTime();
			delta = curtime - lasttime;
			lasttime = curtime;
			
			framesectime += delta;
			frames++;

			if (framesectime >= 1)
			{
				framesectime = 0;
				fps = frames;
				ft = 1.0 / frames;
				frames = 0;
			}

			simtime = 0;
			while (simtime < delta)
			{
				simtime += updatetime;

				pScene->Update(updatetime);
				pScene->Input();
			
				shouldrender = true;
			}

			if (shouldrender)
				Render();
			//else
		//		Sleep(1);
		}
	}

	void Engine::Render()
	{
		pScene->Render(delta);

		char tris[30];
		_itoa(pScreen->triangles, tris, 10);
		char fpss[30];
		snprintf(fpss, 30, "%lf", fps);
		char fts[30];
		snprintf(fts, 30, "%lf", ft);

		pScreen->WriteString(0, 0, FG_RED, "TRIANGLES: ");
		pScreen->WriteString(0, 2, FG_RED, "FPS: ");
		pScreen->WriteString(0, 4, FG_RED, "FT: ");
		pScreen->WriteString(11, 0, FG_RED, tris);
		pScreen->WriteString(11, 2, FG_RED, fpss);
		pScreen->WriteString(11, 4, FG_RED, fts);
		pScreen->DrawFrame();
			
		pScreen->ClearFrame(0);
				
		pScreen->triangles = 0;
	}

	void Engine::CleanUp()
	{

	}

}