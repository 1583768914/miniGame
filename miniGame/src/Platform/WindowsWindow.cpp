#include "hzpch.h"
#include "WindowsWindow.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

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
		// // 2.1window创建窗口创建窗口
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		// 设置glfw当前上下文
		glfwMakeContextCurrent(m_Window);
		/*
	设置窗口关联的用户数据指针。这里GLFW仅做存储，不做任何的特殊处理和应用。
	window表示操作的窗口句柄。
	pointer表示用户数据指针。
*/
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		  // 2.2设置glfw事件回调=接收glfw窗口事件
		// 窗口大小调整事件
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			// glfwGetWindowUserPointer获取void*指针可以转换为由glfwSetWindowUserPointer自定义数据类型
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;

			// 2.3将glfw窗口事件转换为自定义的事件
			WindowResizedEvent event(width, height);
			data.EventCallback(event);
		});
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
