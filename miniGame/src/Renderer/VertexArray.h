#pragma once
#include <memory>
#include "Buffer.h"
#include <vector>


namespace Hazel {
  
	class VertexArray {

	public:
		virtual ~VertexArray() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		//添加顶点缓冲对象
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		// 设置索引缓冲对象，一般只有一个
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;

		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		// 基类指针指向子类对象
		static VertexArray* Create();
	};


}