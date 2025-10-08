#include "./hzpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>


namespace Hazel {

	// VertexBuffer /////////////////////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		//1.������c���n����
		glCreateBuffers(1, &m_RendererID);
		//2.�󶨶��㻺�����
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		//3.�����ǵ�CPU�������ݸ��Ƶ�GPU���㻺���С���OpenGLʹ��
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		//4.初始化BufferLayout（可以根据实际需求设置具体的布局）
	}

	void OpenGLVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {

		glDeleteBuffers(1, &m_RendererID);
	}



	// IndexBuffer //////////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count):m_Count(count) {
		// 1.�������㻺�����
		glCreateBuffers(1, &m_RendererID);
		// 2.�󶨶��㻺�����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		// 3. �������ǵ�CPU���������ݵ�GPU���������У���OpenGLʹ��
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