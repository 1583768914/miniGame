#pragma once
#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{

    class HAZEL_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();
        virtual void OnAttach() {} // 应用添加磁层执行
        virtual void OnDetach() {} // 应用分离此层执行

        virtual void OnUpdate() {} // 每帧更新

        virtual void OnEvent(Event& event) {};
        inline const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };

}