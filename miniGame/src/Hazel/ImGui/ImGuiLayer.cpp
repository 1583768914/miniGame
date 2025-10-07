#include "ImGuiLayer.h"
#include "GLFW/glfw3.h"
#include "Hazel/Application.h"
#include <glad/glad.h>
#include "../../Vendor/imgui/backends/imgui_impl_glfw.h"
#include "../../Vendor/imgui/backends/imgui_impl_opengl3.h"

namespace Hazel
{

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}
    ImGuiLayer::~ImGuiLayer() {}
    // 初始化设置ImGui所有窗口的属性，使ImGui窗口能有停靠、独立的UI窗口特性
    void ImGuiLayer::OnAttach()
    {
        // 不需要手动写ImGui的键值对应GLFW的键值、ImGui接收GLFW窗口事件，ImGui自动完成
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        // 设置ImGui字体 - 解决中文字符乱码问题
        // 清空默认字体
        io.Fonts->Clear();
        
        // 创建字体配置
        ImFontConfig config;
        config.OversampleH = 2;
        config.OversampleV = 1;
        config.PixelSnapH = true;
        
        // 定义字符范围，包含ASCII和中文字符
        static const ImWchar ranges[] =
        {
            0x0020, 0x00FF, // 基本ASCII范围
            0x4E00, 0x9FFF, // 常用中文字符范围
            0,
        };
        
        // 尝试加载Windows系统中的中文字体
        bool fontLoaded = false;
        
        // 尝试加载多种可能的中文字体
        if (!fontLoaded) {
            fontLoaded = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\simhei.ttf", 16.0f, &config, ranges);
        }
        if (!fontLoaded) {
            fontLoaded = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\simsun.ttc", 16.0f, &config, ranges);
        }
        if (!fontLoaded) {
            fontLoaded = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 16.0f, &config, ranges);
        }
        
        // 如果没有找到中文字体，使用默认字体（可能仍会导致中文显示为问号）
        if (!fontLoaded) {
            io.Fonts->AddFontDefault();
        }
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();
        
        // 配置ImGui样式使其窗口背景半透明
        ImGuiStyle &style = ImGui::GetStyle();
        
        // 使ImGui窗口背景半透明，这样后面渲染的三角形就能透过窗口背景显示出来
        style.Colors[ImGuiCol_WindowBg].w = 0.8f; // 将alpha通道设置为0.8（半透明）
        
        // 当启用Viewports时的特殊设置
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            // 保留窗口背景的半透明设置
            // style.Colors[ImGuiCol_WindowBg].w = 1.0f; // 不再设置为完全不透明
        }

        Application &app = Application::Get();
        GLFWwindow *window = static_cast<GLFWwindow *>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer bindings - 使用与着色器相同的OpenGL版本
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
        
        // 重建字体纹理（必须在初始化OpenGL3后端之后调用）
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }
    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void ImGuiLayer::End()
    {
        ImGuiIO &io = ImGui::GetIO();
        Application &app = Application::Get();
        io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
    void ImGuiLayer::OnImGuiRender()
    {
        // 移除停靠空间功能，使用普通独立窗口
        static bool appOpen = true;
        
        // 创建主菜单窗口（可选显示）
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("文件"))
            {
                if (ImGui::MenuItem("退出")) appOpen = false;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // 创建普通独立窗口 1
        ImGui::Begin("窗口 1");
        ImGui::Text("这是第一个独立窗口");
        ImGui::End();

        // 创建普通独立窗口 2
        ImGui::Begin("窗口 2");
        ImGui::Text("这是第二个独立窗口");
        ImGui::End();
    }
}