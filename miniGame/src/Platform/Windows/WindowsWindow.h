#pragma once
#include "../../Window.h"
#include <GLFW/glfw3.h>
#include "Renderer/GraphicsContext.h"

namespace Hazel {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		void OnUpdate() override;
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		// 设置Application的回调函数
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	/// <summary>
	/// 获取原生窗口指针的虚函数
	/// @return 返回指向原生窗口的void指针
	/// </summary>
		virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};
};
