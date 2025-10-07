#include "./hzpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>


namespace Hazel {

	// VertexBuffer /////////////////////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
	   //1.創建頂點緩衝對象
		glCreateBuffers(1, &m_RendererID);
	   //2.绑定顶点缓冲对象
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

	    //3.把我们的CPU顶点数据复制到GPU顶点缓冲中。供OpenGL使用
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {

		glDeleteBuffers(1, &m_RendererID);
	}

	void  OpenGLVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// IndexBuffer //////////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count):m_Count(count) {
		// 1.创建顶点缓冲对象
		glCreateBuffers(1, &m_RendererID);
		// 2.绑定顶点缓冲对象
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		// 3. 复制我们的CPU的索引数据到GPU索引缓冲中，供OpenGL使用
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}