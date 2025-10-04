#include "WindowsInput.h"
#include "../../hzpch.h"
#include "../../Hazel/Application.h"
#include "GLFW/glfw3.h"


namespace Hazel {

	Input* Input::s_Instance = new WindowsInput(); //初始化静态单例实例

	bool WindowsInput::IsKeyPressedImpl(int keycode) {
		//获取GLFW原生窗口void* 并转为GLFWindow*

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		//从当前的GLFW窗口获取状态
		auto state = glfwGetKey(window, keycode);

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
	    auto position = GetMousePositionImpl();
	    return position.first;
	}

	float WindowsInput::GetMouseYImpl() {
	    auto position = GetMousePositionImpl();
	    return position.second;
	}



}