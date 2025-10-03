#include "Hazel/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace Hazel {
    // BIND_EVENT_FN宏定义 - 正确的成员函数绑定语法
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
  
	Application::Application() {	    
	    m_Window = std::unique_ptr<Window>(Window::Create());

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application() {
		// m_Window是std::unique_ptr类型，会自动处理内存释放
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

		HZ_CORE_TRACE("{0}", e.ToString());
    }

    bool Application::OnWindowClose(WindowClosedEvent& e) {
        m_Running = false;
        return true;
    }

    void Application::Run() {
        while (m_Running)
        {
            // 渲染
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Window->OnUpdate(); // 更新glfw
        }
	}
} // namespace Hazel