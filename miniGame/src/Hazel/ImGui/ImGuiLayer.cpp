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

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();
        //  When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application &app = Application::Get();
        GLFWwindow *window = static_cast<GLFWwindow *>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
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
        // 显示一个示例的停靠窗口
        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // 我们使用 ImGuiWindowFlags_NoDocking 来使主窗口没有标题栏，这是停靠空间所必需的
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // 当使用 ImGuiDockNodeFlags_PassthruCentralNode 时，我们希望确保主窗口的填充
        // 不为零，否则我们无法捕获鼠标点击，这些点击将直接传递到绘图后台。
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // 停靠空间
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // 菜单
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("文件"))
            {
                if (ImGui::MenuItem("退出")) dockspaceOpen = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // 停靠窗口示例
        ImGui::Begin("窗口 1");
        ImGui::Text("这是第一个停靠窗口");
        ImGui::End();

        ImGui::Begin("窗口 2");
        ImGui::Text("这是第二个停靠窗口");
        ImGui::End();

        ImGui::End();
    }
}