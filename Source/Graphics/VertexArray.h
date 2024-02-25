#pragma once

namespace graphics { class VertexBufferLayout; }

#include "VertexBuffer.h"

/*
Inserts buffers into specific layout in the vertex shader.
*/


namespace graphics
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(int id) : m_ID(id), attrib_count(0) { }
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		/* Set vertex attribute at 'index' to update once after rendering 'divisor' instances */
		// inline void SetAttribDivisor(unsigned int index, unsigned int divisor = 0);

		void Use() const;
		void Unbind() const;

		inline unsigned int ID() const { return m_ID; }

	private:
		unsigned int m_ID;
		unsigned int attrib_count;
	};
}
