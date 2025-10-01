#pragma once 

#include "hzpch.h"
#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{
    struct WindowProps
    {
        std::string Title;
        uint32_t Width, Height;

        WindowProps(const std::string& title = "Hazel Engine",
            uint32_t width = 1280, uint32_t height = 720)
            : Title(title), Width(width), Height(height) {}
    };

    class HAZEL_API Window{
        public:
          using EventCallbackFn = std::function<void(Event&)>;
    };
};  