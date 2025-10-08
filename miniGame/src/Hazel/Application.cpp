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

        
        //1.生成顶点数据
        m_VertexArray.reset(VertexArray::Create());

        //2.顶点缓冲
        std::shared_ptr<VertexBuffer> vertexBuffer;


        // 2.1顶点缓冲
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        ////////////////////////////////////////////////////////////////////////
        // 2.2 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局/////////////////////////
        ////////////////////////////////////////////////////////////////////////
        
        BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
        };
            
        
        //3.先设置顶点缓冲布局-计算好各个属性所谓的值
        m_VertexBuffer->SetLayout(layout);

        //4.再给顶点数组添加缓冲 设置各个属性的顶点属性指针
        m_VertexArray->AddVertexBuffer(vertexBuffer);
        
        //5.索引缓冲
        std::shared_ptr<IndexBuffer>indexBuffer;

        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        //6.给顶点数组设置索引缓冲
        m_VertexArray->SetIndexBuffer(indexBuffer);


        
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


         
         /*渲染一个quead所做的准备*/
         // 渲染一个quad所做的准备
        // 0.顶点数据
         float squareVertices[3 * 4] = {
             -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
             -0.75f,  0.75f, 0.0f
         };
         uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 }; // 索引数据
         // 1.生成顶点数组对象VAO
         m_SquareVA.reset(VertexArray::Create());
         // 2.顶点缓冲
         std::shared_ptr<VertexBuffer> squareVB;
         squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
         // 设定顶点属性指针，来解释顶点缓冲中的顶点属性布局
         BufferLayout layout2 = {
             { ShaderDataType::Float3, "a_Position" }
         };
         // 3.先设置顶点缓冲布局-计算好各个属性的所需的值
         squareVB->SetLayout(layout2);
         // 4.再给顶点数组添加顶点缓冲-设置各个属性的顶点属性指针
         m_SquareVA->AddVertexBuffer(squareVB);
         // 5.索引缓冲
         std::shared_ptr<IndexBuffer> squareIB;
         squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
         // 6.给顶点数组设置索引缓冲
         m_SquareVA->SetIndexBuffer(squareIB);
         // 着色器代码
         std::string blueShaderVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
         std::string blueShaderFragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
         m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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

            //绘制四边形
            m_BlueShader->Bind(); //绑定着色器
            m_SquareVA->Bind(); //顶点数组对象并且绘制

            glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


            //绘制三角形
            m_Shader->Bind(); //绑定着色器

            m_VertexArray->Bind(); //绑定顶点数组对象
           
            glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        }
    }
} // namespace Hazel