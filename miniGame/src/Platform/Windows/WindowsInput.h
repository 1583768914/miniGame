#pragma once
#include "../../Input.h"


namespace Hazel {
   
	class WindowsInput :public Input {
		

	public:
		// 用于测试GLFW键盘状态的方法
		HAZEL_API void TestGLFWKeyState();
		
		// 静态方法：获取WindowsInput实例
		static WindowsInput* Get() {
			return static_cast<WindowsInput*>(Input::s_Instance);
		}

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float>GetMousePositionImpl() override;

		virtual float GetMouseXImpl() override;

		virtual float GetMouseYImpl() override;
	};


}