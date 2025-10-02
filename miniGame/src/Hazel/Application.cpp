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

	void Application::PushLayer(Layer* layer){
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer){
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e){
		
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>((BIND_EVENT_FN(OnWindowClose));

		for(auto it = m_LayerStack.end();it!=m_LayerStack.begin();){
			 
			(*--it)->OnEvent(e);
			
			if(e.Handled){
				break;
			}
		}
	}

	void Application::Run() {
		while (m_Runing)
		{
			// 渲染
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
   
			for(Layer* layer:m_LayerStack)
			  layer->OnUpdate();

			m_Window->OnUpdate(); // 更新glfw
		}
	}
} // namespace Hazel