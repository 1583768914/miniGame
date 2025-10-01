#include "hzpch.h"
#include "WindowsWindow.h"

namespace Hazel {
	static bool s_GLFWInitialized = false;
	// 通过WindowsWindow类实现Window接口的工厂方法
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}
	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}
	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}
	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
		if (!s_GLFWInitialized) {
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");
			s_GLFWInitialized = true;
		}
		// 创建窗口
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		// 设置glfw当前上下文
		glfwMakeContextCurrent(m_Window);
		// 设置窗口用户指针
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}
	void WindowsWindow::OnUpdate() {
		glfwPollEvents();		// 轮询事件	
		glfwSwapBuffers(m_Window);	// 交换缓冲
	}
	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}
	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}
}
