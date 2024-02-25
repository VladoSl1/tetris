#pragma once

#include "Renderer.h"

#include <iostream>
#include <vector>

namespace graphics
{
	struct VertexBufferLayoutElement
	{
		unsigned int type;
		unsigned int count;
		unsigned int normalized;
		unsigned int divisor = 0;

		static unsigned int SizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
			default: std::cout << "Wrong Type\n"; return 0;
			}
		}
	};

	/*
	Divides data given in vertex buffer into corresponding layout
	in vertex shader.
	*/

	class VertexBufferLayout
	{

	public:
		VertexBufferLayout() : m_Stride(0) { }
		~VertexBufferLayout() { };

		template<typename Type>
		void Add(unsigned int count, unsigned int divisor = 0)
		{
			throw "Adding currently unsuported type!";
		}

		template<>
		void Add<float>(unsigned int count, unsigned int divisor)
		{
			m_Elements.push_back({ GL_FLOAT, count, GL_TRUE, divisor });
			m_Stride += VertexBufferLayoutElement::SizeOfType(GL_FLOAT) * count;
		}

		inline unsigned int GetStride() const { return m_Stride; }
		inline std::vector<VertexBufferLayoutElement> GetElements() const { return m_Elements; }

	private:
		std::vector<VertexBufferLayoutElement> m_Elements;
		unsigned int m_Stride;
	};
}

