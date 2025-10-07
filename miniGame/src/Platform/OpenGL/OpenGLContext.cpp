#include "hzpch.h"
#include "Hazel/Core.h"
#include "Hazel/Log.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
{
	//将我们窗口的上下文设置为当前线程的上下文
	glfwMakeContextCurrent(m_WindowHandle);

	//获取显卡openGL函数定义的地址
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

	// 正确输出OpenGL信息
	HZ_CORE_INFO("OpenGL.信息:");
	HZ_CORE_INFO("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
	HZ_CORE_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
	HZ_CORE_INFO("Version: {0}", (const char*)glGetString(GL_VERSION));
}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle); // 交换缓冲区
	}

}