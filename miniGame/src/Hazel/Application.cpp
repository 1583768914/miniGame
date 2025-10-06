#include "Hazel/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"
#include "Window.h"
#include "Input.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>



namespace Hazel {
    // BIND_EVENT_FN宏定义 - 正确的成员函数绑定语法
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
  
	Application* Application::s_Instance = nullptr;

	Application::Application() {	  
        
	    HZ_CORE_ASSERT(!s_Instance, "引用已经存在");
		s_Instance = this;

		 // 1.1Application创建窗口
         m_Window = std::unique_ptr<Window>(Window::Create());
         // 1.2Application设置窗口事件的回调函数
          m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	
           // 将ImGui层放在最后
            m_ImGuiLayer = new ImGuiLayer();
            PushOverlay(m_ImGuiLayer);
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

		// 将事件从后往前传递给所有层
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			--it;
			if (e.IsHandled())
				break;
			(*it)->OnEvent(e); // 启用事件传递给各层
		}

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

            // 获取并记录鼠标位置
            // std::pair<float, float> mousePos = Input::GetMousePosition();
            // HZ_CORE_TRACE("{0}, {1}", mousePos.first, mousePos.second);

            // 先更新所有层
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();
            
            // 开始ImGui帧
            m_ImGuiLayer->Begin();
            
            // 然后渲染所有层的ImGui内容
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            
            // 结束ImGui帧并渲染
            m_ImGuiLayer->End();
            
            // 最后更新窗口
            m_Window->OnUpdate(); // 更新glfw
        }
	}
} // namespace Hazel