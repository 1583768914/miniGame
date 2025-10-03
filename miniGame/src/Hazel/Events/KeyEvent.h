#pragma once

#include "Event.h"

namespace Hazel {
	class HAZEL_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; };

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keyCode) : m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class HAZEL_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class HAZEL_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}
		
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	} // namespace Hazel

// 为Hazel命名空间下的事件类提供fmt库支持
// 这样可以在spdlog中直接使用这些类对象作为日志参数
namespace fmt {
    template <>
    struct formatter<Hazel::KeyPressedEvent> : formatter<std::string> {
        template <typename FormatContext>
        auto format(const Hazel::KeyPressedEvent& e, FormatContext& ctx) const -> decltype(ctx.out()) {
            return formatter<std::string>::format(e.ToString(), ctx);
        }
    };
    
    template <>
    struct formatter<Hazel::KeyReleasedEvent> : formatter<std::string> {
        template <typename FormatContext>
        auto format(const Hazel::KeyReleasedEvent& e, FormatContext& ctx) const -> decltype(ctx.out()) {
            return formatter<std::string>::format(e.ToString(), ctx);
        }
    };
}