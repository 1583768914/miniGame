#pragma once


#include "Layer/Layer.h"


namespace Hazel {
   
	// 前置声明
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseMovedEvent;
	class MouseScrolledEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class WindowResizedEvent;

	class HAZEL_API ImGuiLayer :public Layer
	{
	   public:
		   ImGuiLayer();
		   ~ImGuiLayer();


		   void OnAttach();
		   void OnDetach();
		   void OnUpdate();
		   void OnEvent(Event& event);

	    private:
		float m_Time = 0.0f;

		// 事件处理函数
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleaseEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnWindowResizedEvent(WindowResizedEvent& e);
	};


}