// SandboxApp.cpp - 沙盒应用程序实现

// 为了确保UTF-8编码支持，我们在代码文件中添加必要的编译指令
#ifdef _WIN32
#pragma execution_character_set("utf-8") // 启用UTF-8编码支持
#endif

#include <Hazel.h>
#include <Hazel/EntryPoint.h>
#include <Input.h>
#include <Hazel/KeyCodes.h>
#include <Hazel/Events/KeyEvent.h>

class ExampleLayer:public Hazel::Layer
{
   public:
     ExampleLayer():Layer("Example"){};

     void  OnUpdate() override
     {
          //HZ_INFO("ExapleLayer::Update"); //最终会被输出
         if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
             HZ_TRACE("A键按下(poll)");
         }
     }

    

     void OnEvent(Hazel::Event& event)override{
        //HZ_TRACE("{0}",event.ToString());
         if (event.GetEventType() == Hazel::EventType::KeyPressed) {
             Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
             if (e.GetKeyCode() == HZ_KEY_A) {// 使用键值，检测A键是否按下
                 HZ_TRACE("A键按下(event)");
             }
             HZ_TRACE("{0}", (char)e.GetKeyCode());
         }
     }
     
};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        // 初始化应用程序
        PushLayer(new ExampleLayer());
        //PushLayer(new Hazel::DebugLayer()); // 添加调试层
        PushOverlay(new Hazel::ImGuiLayer());// UI层放到最后面显示在屏幕的上方
        
    }

    ~Sandbox()
    {
        // 清理资源
    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new Sandbox();
}