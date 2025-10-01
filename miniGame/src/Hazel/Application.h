#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"


namespace Hazel {
  
	class HAZEL_API Application
	{
	   public:
		 Application();
	     virtual ~Application();

		 void Run();

	private:
	Window* m_Window;
	bool m_Runing = true;

	};




	//To be defined in CLIENT
	Application* CreateApplication();

}