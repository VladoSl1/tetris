#include "VertexBuffer.h"

#include "Renderer.h"

namespace graphics
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
		: m_ID(0)
	{
		GLCall(glGenBuffers(1, &m_ID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}
	
	VertexBuffer::VertexBuffer(unsigned int size)
		: m_ID(0)
	{
		GLCall(glGenBuffers(1, &m_ID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW));
	}

	void VertexBuffer::BufferSubData(const void* data, unsigned int size, unsigned int offset)
	{
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_ID));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::Use() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	}
}