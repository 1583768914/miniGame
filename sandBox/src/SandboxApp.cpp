// SandboxApp.cpp - 沙盒应用程序实现

// 为了确保UTF-8编码支持，我们在代码文件中添加必要的编译指令
#ifdef _WIN32
#pragma execution_character_set("utf-8") // 启用UTF-8编码支持
#endif

#include <Hazel.h>
#include <Hazel/EntryPoint.h>

class ExampleLayer:public Hazel::Layer
{
   public:
     ExampleLayer():Layer("Example"){};

     void  OnUpdate() override
     {
          HZ_INFO("ExapleLayer::Update"); //最终会被输出
     }

     void OnEvent(Hazel::Event& event)override{
        HZ_TRACE("{0}",event);
     }
     
};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        // 初始化应用程序
        PushLayer(new ExampleLayer());
        
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