#include "ImGuiLayer.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include "Hazel/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include <glad/glad.h>
#include "../../Vendor/imgui/backends/imgui_impl_glfw.h"

// 添加HZ_BIND_EVENT_FN宏定义
#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{}

	ImGuiLayer::~ImGuiLayer()
	{}

	void ImGuiLayer::OnAttach()
{
    // 创建ImGui上下文
    ImGui::CreateContext();
    
    // 设置ImGui样式
    ImGui::StyleColorsDark();

    // 获取ImGui IO
    ImGuiIO& io = ImGui::GetIO();
    
    // 设置后端标志
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;     // 支持光标
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;     // 支持设置鼠标位置
    
    // 禁用多视口功能，避免上下文冲突
    io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
    
    // 明确声明使用旧的键盘输入API（KeyMap和KeysDown数组）
    #ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
    io.BackendUsingLegacyKeyArrays = -1;
    #endif

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

    // 获取GLFW窗口指针并进行安全检查
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window) {
        // 如果窗口不存在，可以考虑使用其他方式获取或者设置一个标志表示ImGui未初始化
        return;
    }

    // 初始化GLFW和OpenGL3后端
    // 设置install_callbacks为false，避免GLFW后端使用新的键盘API
    // 我们在ImGuiLayer的事件处理函数中使用旧的键盘API（KeyMap和KeysDown）
    ImGui_ImplGlfw_InitForOpenGL(window, false);
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
    // 获取ImGui IO
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    
    // 设置显示尺寸，添加明确的类型转换解决警告
    io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));

    // 更新时间
    float time = (float)glfwGetTime();
    io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
    m_Time = time;

    // 开始新的ImGui帧
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    // 显示ImGui Demo窗口
    static bool show = true;
    ImGui::ShowDemoWindow(&show);
    
    // 渲染ImGui内容
    ImGui::Render();
    
    // 确保GetDrawData不为空才渲染
    ImDrawData* drawData = ImGui::GetDrawData();
    if (drawData) {
        // 渲染ImGui数据
        ImGui_ImplOpenGL3_RenderDrawData(drawData);
    }
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