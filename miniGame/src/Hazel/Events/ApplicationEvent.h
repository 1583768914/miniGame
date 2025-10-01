#pragma once

#include "Event.h"

namespace Hazel {

	class HAZEL_API ApplicationEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	protected:
		ApplicationEvent() {}
	};

	class HAZEL_API AppTickEvent : public ApplicationEvent {
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
	};

	class HAZEL_API AppUpdateEvent : public ApplicationEvent {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
	};

	class HAZEL_API AppRenderEvent : public ApplicationEvent {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
	};

	class HAZEL_API WindowEvent : public ApplicationEvent {
	public:
		EVENT_CLASS_CATEGORY(ApplicationEvent::GetCategoryFlags() | EventCategoryWindow)
	protected:
		WindowEvent() {}
	};

	class HAZEL_API WindowMovedEvent : public WindowEvent {
	public:
		WindowMovedEvent(float windowX, float windowY)
			: m_WindowX(windowX), m_WindowY(windowY) {}

		inline float GetWindowX() const { return m_WindowX; }
		inline float GetWindowY() const { return m_WindowY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_WindowX << ", " << m_WindowY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
	private:
		float m_WindowX, m_WindowY;
	};

	class HAZEL_API WindowResizedEvent : public WindowEvent {
	public:
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizedEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized)
	protected:
		unsigned int m_Width, m_Height;
	};

	class HAZEL_API WindowClosedEvent : public WindowEvent {
	public:
		WindowClosedEvent() {}
		
		EVENT_CLASS_TYPE(WindowClosed)
	};

	class HAZEL_API WindowFocusedEvent : public WindowEvent {
	public:
		WindowFocusedEvent() {}

		EVENT_CLASS_TYPE(WindowFocused)
	};

	class HAZEL_API WindowLostFocusEvent : public WindowEvent {
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus)
	};
}

// Register customized classes to spdlog
HZ_LOG_FORMAT_CLASS(Hazel::AppTickEvent)
HZ_LOG_FORMAT_CLASS(Hazel::AppUpdateEvent)
HZ_LOG_FORMAT_CLASS(Hazel::AppRenderEvent)
HZ_LOG_FORMAT_CLASS(Hazel::WindowResizedEvent)
HZ_LOG_FORMAT_CLASS(Hazel::WindowMovedEvent)
HZ_LOG_FORMAT_CLASS(Hazel::WindowClosedEvent)
HZ_LOG_FORMAT_CLASS(Hazel::WindowFocusedEvent)
HZ_LOG_FORMAT_CLASS(Hazel::WindowLostFocusEvent)