#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Layer/LayerStack.h"
#include "Layer/Layer.h"

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

    private:
        Window* m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        bool OnWindowClose(Event& e);
    };

    //To be defined in CLIENT
    Application* CreateApplication();

}