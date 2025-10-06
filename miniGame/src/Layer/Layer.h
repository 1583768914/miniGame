#pragma once
#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

// 局部禁用C4251警告，这是关于STL容器在DLL导出类中使用的警告
#pragma warning(push)
#pragma warning(disable : 4251)

namespace Hazel
{

    class HAZEL_API Layer
    {
    public:
        Layer(const std::string &name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}      // 应用添加此层执行
        virtual void OnDetach() {}      // 应用分离此层执行
        virtual void OnUpdate() {}      // 每层更新
        virtual void OnImGuiRender() {} // 每层都可以拥有自己的UI窗口 !
        virtual void OnEvent(Event&) {}  // 每层事件处理

    protected:
        std::string m_DebugName;

        
    };

} // namespace Hazel

// 恢复之前的警告设置
#pragma warning(pop)