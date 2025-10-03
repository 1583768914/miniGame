#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Layer/LayerStack.h"
#include "Layer/Layer.h"
#include <memory>

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

		inline Application& Get(){
			return *s_Instacne;
		}

		inline Window& GetWindow() {return *m_Window;}

    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        bool OnWindowClose(WindowClosedEvent& e);

	private:
	   
	    static Application* s_Instance;
    };



    //To be defined in CLIENT
    Application* CreateApplication();

};

// 恢复之前的警告设置
#pragma warning(pop)