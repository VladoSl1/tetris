#pragma once

/*
Saves data (coordinates, colors, texture coordinates...)
for each vertex.
*/


namespace graphics
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		VertexBuffer(unsigned int size);
		VertexBuffer(int id) : m_ID(id) { }
		~VertexBuffer();

		void Use() const;
		void Unbind() const;

		void BufferSubData(const void* data, unsigned int size, unsigned int offset = 0);

		inline unsigned int ID() const { return m_ID; }

	private:
		unsigned int m_ID;
	};
}