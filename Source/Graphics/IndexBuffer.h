#pragma once

#include "Renderer.h"

/*
IndexBuffer is used for vertex management.
It matches coordinates to triangle.
Prevention against repeating coordinates (performance).
*/

namespace graphics
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, unsigned int count)
			:m_ID(0), m_Count(count)
		{
			GLCall(glGenBuffers(1, &m_ID));
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
		}

		~IndexBuffer()
		{
			GLCall(glDeleteBuffers(1, &m_ID));
		}

		/* Set as active */
		void Use() const
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
		}

		void Unbind() const
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		inline unsigned int GetCount() const { return m_Count; }
		inline unsigned int ID() const { return m_ID; }

	private:
		unsigned int m_ID;
		unsigned int m_Count;
	};
}


