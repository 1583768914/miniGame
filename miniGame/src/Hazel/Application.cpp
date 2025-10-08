#include "Hazel/Application.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"
#include "Window.h"
#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hazel
{

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
        case Hazel::ShaderDataType::Float:    return GL_FLOAT;
        case Hazel::ShaderDataType::Float2:   return GL_FLOAT;
        case Hazel::ShaderDataType::Float3:   return GL_FLOAT;
        case Hazel::ShaderDataType::Float4:   return GL_FLOAT;
        case Hazel::ShaderDataType::Mat3:     return GL_FLOAT;
        case Hazel::ShaderDataType::Mat4:     return GL_FLOAT;
        case Hazel::ShaderDataType::Int:      return GL_INT;
        case Hazel::ShaderDataType::Int2:     return GL_INT;
        case Hazel::ShaderDataType::Int3:     return GL_INT;
        case Hazel::ShaderDataType::Int4:     return GL_INT;
        case Hazel::ShaderDataType::Bool:     return GL_BOOL;
        }
        return 0;
    }


// BIND_EVENT_FN宏定义 - 正确的成员函数绑定语法
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application *Application::s_Instance = nullptr;

    Application::Application()
    {

        HZ_CORE_ASSERT(!s_Instance, "引用已经存在");
        s_Instance = this;

        // 1.1Application创建窗口
        m_Window = std::unique_ptr<Window>(Window::Create());
        // 1.2Application设置窗口事件的回调函数
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        // 启用OpenGL混合功能，这对于ImGui窗口的半透明显示至关重要
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // 将ImGui层放在最后
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        // 使用OpenGL函数渲染一个三角形
        float vertices[3 * 7] = {
       -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
       0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
       0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };


        unsigned int indices[3] = {0, 1, 2}; // 索引数据

        // 0.生成顶点数组对象VAO、顶点缓冲对象VBO、索引缓冲对象EBO
        glGenVertexArrays(1, &m_VertexArray);


        // 1. 绑定顶点数组对象
        glBindVertexArray(m_VertexArray);

        //2.1頂點緩衝
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        // 2.1顶点缓冲
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        ////////////////////////////////////////////////////////////////////////
        // 2.2 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局/////////////////////////
        ////////////////////////////////////////////////////////////////////////
        {
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
            };
            m_VertexBuffer->SetLayout(layout);
        }

        //:临时的，会放在顶点数组抽象类中
        uint32_t index = 0;

        const auto& layout = m_VertexBuffer->GetLayout();

        for (const auto& element :layout) {
          
            glEnableVertexAttribArray(index);

            glVertexAttribPointer(index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);

            index++;
        }


        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        
        // 4. 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局
        //glEnableVertexAttribArray(0); // 开启glsl的layout = 0输入
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        // 注意：在现代OpenGL (>=3.0) 核心模式中必须使用着色器程序
        // 下面是最简化的着色器实现
        std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
        std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

         m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

        // 编译和链接最简化的着色器程序
        // unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        // glCompileShader(vertexShader);

        // // 检查顶点着色器编译错误
        // int success;
        // char infoLog[512];
        // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        // if (!success)
        // {
        //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        //     HZ_CORE_ERROR("顶点着色器编译失败: {0}", infoLog);
        // }

        // unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        // glCompileShader(fragmentShader);

        // // 检查片段着色器编译错误
        // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        // if (!success)
        // {
        //     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        //     HZ_CORE_ERROR("片段着色器编译失败: {0}", infoLog);
        // }

        // m_ShaderProgram = glCreateProgram();
        // glAttachShader(m_ShaderProgram, vertexShader);
        // glAttachShader(m_ShaderProgram, fragmentShader);
        // glLinkProgram(m_ShaderProgram);

        // // 检查着色器程序链接错误
        // glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
        // if (!success)
        // {
        //     glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
        //     HZ_CORE_ERROR("着色器程序链接失败: {0}", infoLog);
        // }

        // glDeleteShader(vertexShader);
        // glDeleteShader(fragmentShader);
    }

    Application::~Application()
    {
        // m_Window是std::unique_ptr类型，会自动处理内存释放
    }

    void Application::PushLayer(Layer *layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *layer)
    {
        m_LayerStack.PushOverlay(layer);
    }

    void Application::OnEvent(Event &e)
    {

        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(OnWindowClose));

        // 将事件从后往前传递给所有层
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            --it;
            if (e.IsHandled())
                break;
            (*it)->OnEvent(e); // 启用事件传递给各层
        }

        HZ_CORE_TRACE("{0}", e.ToString());
    }

    bool Application::OnWindowClose(WindowClosedEvent &e)
    {
        m_Running = false;
        return true;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // 先清除屏幕
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // 绑定着色器
            m_Shader->Bind();

            // 渲染三角形 - 在任何层更新和ImGui渲染之前
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

            // 更新所有层
            for (Layer *layer : m_LayerStack)
                layer->OnUpdate();

            // 开始ImGui帧
            m_ImGuiLayer->Begin();

            // 然后渲染所有层的ImGui内容
            for (Layer *layer : m_LayerStack)
                layer->OnImGuiRender();

            // 结束ImGui帧并渲染
            m_ImGuiLayer->End();

            // 最后更新窗口
            m_Window->OnUpdate(); // 更新glfw
        }
    }
} // namespace Hazel