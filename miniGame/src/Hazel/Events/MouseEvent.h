#pragma once

#include "Event.h"

namespace Hazel {

	class HAZEL_API MouseEvent : public Event {
	public:
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseEvent() {}
	};

	class HAZEL_API MouseMovedEvent : public MouseEvent {
	public:
		MouseMovedEvent(float mouseX, float mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY) {}

		inline float GetMouseX() const { return m_MouseX; }
		inline float GetMouseY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
	private:
		float m_MouseX, m_MouseY;
	};

	class HAZEL_API MouseScrolledEvent : public MouseEvent {
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY) {}

		inline float GetOffsetX() const { return m_OffsetX; }
		inline float GetOffsetY() const { return m_OffsetY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		float m_OffsetX, m_OffsetY;
	};

	class HAZEL_API MouseButtonEvent : public MouseEvent {
	public:
		inline int GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(MouseEvent::GetCategoryFlags() | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class HAZEL_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class HAZEL_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
} // namespace Hazel

// 为Hazel命名空间下的事件类提供fmt库支持
// 这样可以在spdlog中直接使用这些类对象作为日志参数
namespace fmt {
    template <>
    struct formatter<Hazel::MouseMovedEvent> : formatter<std::string> {
        template <typename FormatContext>
        auto format(const Hazel::MouseMovedEvent& e, FormatContext& ctx) const -> decltype(ctx.out()) {
            return formatter<std::string>::format(e.ToString(), ctx);
        }
    };
    
    template <>
    struct formatter<Hazel::MouseScrolledEvent> : formatter<std::string> {
        template <typename FormatContext>
        auto format(const Hazel::MouseScrolledEvent& e, FormatContext& ctx) const -> decltype(ctx.out()) {
            return formatter<std::string>::format(e.ToString(), ctx);
        }
    };
    
    template <>
    struct formatter<Hazel::MouseButtonPressedEvent> : formatter<std::string> {
        template <typename FormatContext>
        auto format(const Hazel::MouseButtonPressedEvent& e, FormatContext& ctx) const -> decltype(ctx.out()) {
            return formatter<std::string>::format(e.ToString(), ctx);
        }
    };
    
    template <>
    struct formatter<Hazel::MouseButtonReleasedEvent> : formatter<std::string> {
        template <typename FormatContext>
        auto format(const Hazel::MouseButtonReleasedEvent& e, FormatContext& ctx) const -> decltype(ctx.out()) {
            return formatter<std::string>::format(e.ToString(), ctx);
        }
    };
}