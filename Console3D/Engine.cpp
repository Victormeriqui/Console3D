#include "Engine.hpp"

#include "Time.hpp"
#include "Color.hpp"

using namespace Render;

#define FRAMECAP 5000

namespace Base
{

	Engine::Engine(Screen* screen, Scene* scene, HANDLE* hOut, HANDLE* hIn) :
		pScreen(screen), pScene(scene), pHout(hOut), pHin(hIn)
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

		bool shouldrender = false;

		INPUT_RECORD* eventbuffer;

		while (running)
		{
			curtime = Time::GetCurTime();
			delta = curtime - lasttime;
			lasttime = curtime;
			
			simtime = 0;
			while (simtime < delta)
			{
				simtime += updatetime;

				pScene->Update(updatetime);
				
				int events_c = GetInput(&eventbuffer);
							
				if (events_c)
				{
					for (int i = 0; i < events_c; i++)
					{
						if (eventbuffer[i].EventType == KEY_EVENT || eventbuffer[i].EventType == MOUSE_EVENT)
							pScene->Input(&eventbuffer[i]);				
					}
				}
				
				shouldrender = true;
			}

			if (shouldrender)
				Render();
			else
				Sleep(1);
		}
	}

	void Engine::Render()
	{

		pScene->Render(delta);
		
		pScreen->DrawFrame();
		
		
		pScreen->ClearFrame(COL_BACKGROUND);
	}

	void Engine::CleanUp()
	{

	}

	DWORD Engine::GetInput(INPUT_RECORD** eventbuffer)
	{
		DWORD events_c = 0;
		DWORD retevents_c = 0;

		GetNumberOfConsoleInputEvents(*pHin, &events_c);
		
		if (events_c)
		{
			*eventbuffer = new INPUT_RECORD[events_c];
						
			ReadConsoleInput(*pHin, *eventbuffer, events_c, &retevents_c);
		}

		return retevents_c;
	}

}