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

        // 使用OpenGL函数渲染一个三角形
        float vertices[3 * 3] = {
           -0.5f, -0.5f, 0.0f,
           0.5f, -0.5f, 0.0f,
           0.0f, 0.5f, 0.0f
        };

        unsigned int indices[3] = { 0, 1, 2 }; // 索引数据

        // 0.生成顶点数组对象VAO、顶点缓冲对象VBO、索引缓冲对象EBO
        glGenVertexArrays(1, &m_VertexArray);
        glGenBuffers(1, &m_VertexBuffer);
        glGenBuffers(1, &m_IndexBuffer);
    
        // 1. 绑定顶点数组对象
        glBindVertexArray(m_VertexArray);

        // 2. 把我们的CPU的顶点数据复制到GPU顶点缓冲中，供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 3. 复制我们的CPU的索引数据到GPU索引缓冲中，供OpenGL使用
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // 4. 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局
        glEnableVertexAttribArray(0);// 开启glsl的layout = 0输入
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
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
            // 先清除屏幕
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // 更新所有层
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();
            
            // 开始ImGui帧
            m_ImGuiLayer->Begin();
            
            // 然后渲染所有层的ImGui内容
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            
            // 渲染三角形 - 放在ImGui渲染之前
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
            
            // 结束ImGui帧并渲染
            m_ImGuiLayer->End();
            
            // 最后更新窗口
            m_Window->OnUpdate(); // 更新glfw
        }
	}
} // namespace Hazel