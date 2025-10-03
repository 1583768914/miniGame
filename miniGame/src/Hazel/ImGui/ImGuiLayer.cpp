#include "ImGuiLayer.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include "Hazel/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include <glad/glad.h>
#include "../../Vendor/imgui/backends/imgui_impl_glfw.h"
#include <cstddef> // 用于offsetof宏

// 添加HZ_BIND_EVENT_FN宏定义
#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{}

	ImGuiLayer::~ImGuiLayer()
	{}

	// 前向声明WindowsWindow类
	class WindowsWindow;

	void ImGuiLayer::OnAttach()
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;     // 光标
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // imgui输入key对应glfw的key，临时的：最终会对应引擎自身的key
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    // 从Application获取Window对象
    Application& app = Application::Get();
    Window& window = app.GetWindow();

    // 安全地获取GLFWwindow指针
    WindowsWindow* windowsWindow = static_cast<WindowsWindow*>(&window);
    // 获取WindowsWindow类中的GLFWwindow*成员
    GLFWwindow* glfwWindow = nullptr;
    // 使用反射或其他方式获取私有成员
    // 这里我们使用一个技巧：通过memcpy获取私有成员
    memcpy(&glfwWindow, reinterpret_cast<char*>(windowsWindow) + offsetof(WindowsWindow, m_Window), sizeof(GLFWwindow*));

    // 设置平台/渲染器
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

	void ImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate()
{
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    // 添加明确的类型转换解决警告
    io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));

    float time = (float)glfwGetTime();
    io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
    m_Time = time;

    // 需创建窗口后才执行下面
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    // 显示ImGui Demo
    ImGui::ShowDemoWindow(&show);
    
    // 确保在渲染前完成所有ImGui的绘制命令
    ImGui::Render();
    
    // 渲染ImGui数据
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // 确保不启用视口功能，因为这可能导致上下文冲突
    // 临时禁用多视口功能以避免崩溃
	}

	void ImGuiLayer::OnEvent(Event& event) 
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleaseEvent));
    dispatcher.Dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
    dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
    dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<KeyReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
    dispatcher.Dispatch<WindowResizedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
  }

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[e.GetButton()] = true;
    return false;
}

	bool ImGuiLayer::OnMouseButtonReleaseEvent(MouseButtonReleasedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[e.GetButton()] = false;
    return false;
}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(e.GetMouseX(), e.GetMouseY());
    return false;
}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += e.GetOffsetX();
    io.MouseWheel += e.GetOffsetY();
    return false;
}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[e.GetKeyCode()] = true;

    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

    return false;
}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[e.GetKeyCode()] = false;
    return false;
}


bool ImGuiLayer::OnWindowResizedEvent(WindowResizedEvent& e)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    glViewport(0, 0, e.GetWidth(), e.GetHeight());
    return false;
}
}