// 为了确保UTF-8编码支持，我们在代码文件中添加必要的编译指令
#ifdef _WIN32
#pragma execution_character_set("utf-8") // 启用UTF-8编码支持
#endif

#pragma once 

#include "hzpch.h"
#include "Hazel/Core.h"
#include <Hazel/Events/Event.h>

namespace Hazel {
	struct WindowProps { // 用于初始化窗口的属性

		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowProps(const std::string& title = "Hazel Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};
	class HAZEL_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		// 创建Window接口的实例

		static Window* Create(const WindowProps& props = WindowProps());
	};
}