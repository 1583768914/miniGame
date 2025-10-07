#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Layer/LayerStack.h"
#include "Layer/Layer.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"

#include <memory>
#include <Platform/OpenGL/OpenGLBuffer.h>

// 禁用C4251警告：类"std::unique_ptr<Hazel::Window,std::default_delete<Hazel::Window>>"需要有dll接口由class"Hazel::Application"的客户端使用
#pragma warning(push)
#pragma warning(disable: 4251)

namespace Hazel {
    class HAZEL_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void OnEvent(Event& e);

		static inline Application& Get(){
			return *s_Instance;
		}

		inline Window& GetWindow() {return *m_Window;}

    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer; // 拥有ImGuiLayer控制权
        bool m_Running = true;
        LayerStack m_LayerStack;
        bool OnWindowClose(WindowClosedEvent& e); // 处理窗口关闭事件的函数

        static Application* s_Instance;

        std::unique_ptr<Shader> m_Shader;
        
        unsigned int  m_VertexArray, m_VertexBuffer, m_IndexBuffer;

        unsigned int m_VertexArray;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
    };





    //To be defined in CLIENT
    Application* CreateApplication();

};

// 恢复之前的警告设置
#pragma warning(pop)