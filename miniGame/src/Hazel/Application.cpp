#include "Application.h"
#include "Hazel/Events/ApplicationEvent.h" // �����¼�
#include "Hazel/Log.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace Hazel {
  
	Application::Application() {
	   
		m_Window = Window::Create(); // 创建窗口实例
	}

	Application::~Application() {
		delete m_Window; // 释放窗口资源
	}

	void Application::Run() {
		while (m_Runing)
		{
			// 渲染
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate(); // 更新glfw
		}
	}
} // namespace Hazel