#include "Hazel/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace Hazel {
    // BIND_EVENT_FN宏定义 - 正确的成员函数绑定语法
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
  
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

        dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(OnWindowClose));

        for(auto it = m_LayerStack.end();it!=m_LayerStack.begin();){
              
            (*--it)->OnEvent(e);
            
            if(e.IsHandled()){
                break;
            }
        }
    }

    bool Application::OnWindowClose(Event& e) {
        m_Running = false;
        return true;
    }

    void Application::Run() {
        while (m_Running)
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