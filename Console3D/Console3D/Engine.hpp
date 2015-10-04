#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <Windows.h>

#include "Screen.hpp"
#include "Scene.hpp"

using namespace Render;
using namespace Game;

namespace Base
{
	class Engine
	{

	private:

		const HANDLE* pHout;
		
		Screen* pScreen;
		Scene* pScene;

		bool running;

		double delta;
		double fps;
		double ft;

		void Run();
		void Render();
		void CleanUp();
		
	public:
	
		Engine(Screen* screen, Scene* scene, HANDLE* hOut);
		~Engine();

		void Start();
		void Stop();
		
	};
}

#endif