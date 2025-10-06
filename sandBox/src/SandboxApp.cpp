// SandboxApp.cpp - 沙盒应用程序实现

// 为了确保UTF-8编码支持，我们在代码文件中添加必要的编译指令
#ifdef _WIN32
#pragma execution_character_set("utf-8") // 启用UTF-8编码支持
#endif

#include <Hazel.h>
#include <Hazel/EntryPoint.h>
#include "Platform/Windows/WindowsInput.h"
#include <Input.h>
#include <Hazel/KeyCodes.h>
#include <Hazel/Events/KeyEvent.h>
#include <iostream>
#include <windows.h> // For console encoding settings

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}


class ExampleLayer:public Hazel::Layer
{
   public:
     ExampleLayer():Layer("Example"){
       
         camera(3.0f, { 2.0f, 1.0f });
     
     };

     void  OnUpdate() override
     {// 添加无条件日志，确认方法被调用
         if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {// 使用键值，检测A键是否按下
			HZ_TRACE("A键按下(poll)");
		}
         
         // 使用静态方法获取WindowsInput实例并调用测试方法
         Hazel::WindowsInput::Get()->TestGLFWKeyState();
     }

    

     void OnEvent(Hazel::Event& event)override{
        // Unconditional log to confirm OnEvent method is called
        HZ_CORE_INFO("ExampleLayer::OnEvent called, event type: {0}", event.GetName());
        
        // Print complete event information
        HZ_CORE_INFO("Event details: {0}", event.ToString());
        
        if (event.GetEventType() == Hazel::EventType::KeyPressed) {
            Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
            HZ_CORE_INFO("Key press event detected, keycode: {0}, ASCII: '{1}'", e.GetKeyCode(), (char)e.GetKeyCode());
            
            if (e.GetKeyCode() == HZ_KEY_A) {
                HZ_CORE_INFO("A key pressed! Keycode: {0}", HZ_KEY_A);
            }
        }
        else if (event.GetEventType() == Hazel::EventType::KeyReleased) {
            Hazel::KeyReleasedEvent& e = (Hazel::KeyReleasedEvent&)event;
            HZ_CORE_INFO("Key release event detected, keycode: {0}, ASCII: '{1}'", e.GetKeyCode(), (char)e.GetKeyCode());
            
            if (e.GetKeyCode() == HZ_KEY_A) {
                HZ_CORE_INFO("A key released! Keycode: {0}", HZ_KEY_A);
            }
        }
     }
     
};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        // Set console encoding to UTF-8 to ensure proper character display
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        
        // Enable virtual terminal processing for better console output
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
        
        PushLayer(new ExampleLayer());
        // ImGuiLayer is already created in Application base class
    }

    ~Sandbox()
    {
    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new Sandbox();
}