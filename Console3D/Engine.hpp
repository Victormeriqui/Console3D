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

		HANDLE* pHout;
		HANDLE* pHin;

		Screen* pScreen;
		Scene* pScene;

		bool running;

		double delta;

		void Run();
		void Render();
		void CleanUp();
		
		DWORD GetInput(INPUT_RECORD** eventbuffer);

	public:
	
		Engine(Screen* screen, Scene* scene, HANDLE* hOut, HANDLE* hIn);
		~Engine();

		void Start();
		void Stop();
		
	};
}

#endif