#include "WindowsInput.h"
#include "../../hzpch.h"
#include "../../Hazel/Application.h"
#include "../../Hazel/KeyCodes.h" // 添加KeyCodes.h头文件引用
#include "GLFW/glfw3.h"


namespace Hazel {

	Input* Input::s_Instance = new WindowsInput(); //初始化静态单例实例

	// 直接使用GLFW API测试键盘状态的函数
	void WindowsInput::TestGLFWKeyState() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		if (window) {
			// 直接检查A键状态，不通过IsKeyPressedImpl
			int state = glfwGetKey(window, GLFW_KEY_A);
			HZ_INFO("GLFW直接检测: A键状态 = {0}, GLFW_PRESS={1}", state, GLFW_PRESS);
		}
	}

	bool WindowsInput::IsKeyPressedImpl(int keycode) {
		//获取GLFW原生窗口void* 并转为GLFWindow*

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		// 打印窗口指针调试信息（转换为void*以避免spdlog格式化错误）
		HZ_INFO("WindowsInput: 窗口指针 = {0}", static_cast<void*>(window));

		//从当前的GLFW窗口获取状态
		auto state = glfwGetKey(window, keycode);

		// 添加详细调试日志，记录keycode和状态
		if (keycode == HZ_KEY_A) { // 仅跟踪A键
			HZ_INFO("WindowsInput: A键state check - code={0}, state={1}, GLFW_PRESS={2}, GLFW_RELEASE={3}", 
				keycode, state, GLFW_PRESS, GLFW_RELEASE);
		}

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button) {
	   
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}


	std::pair<float, float>WindowsInput::GetMousePositionImpl() {
	
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}


	float WindowsInput::GetMouseXImpl() {
		auto x = GetMousePositionImpl();
		return std::get<0>(x);
	}

	float WindowsInput::GetMouseYImpl() {
		auto x = GetMousePositionImpl();
		return std::get<1>(x);
	}



}