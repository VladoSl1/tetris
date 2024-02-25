#include "VertexArray.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"

#include <vector>

namespace graphics
{
	VertexArray::VertexArray()
		: m_ID(0), attrib_count(0)
	{
		GLCall(glGenVertexArrays(1, &m_ID));
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_ID));
	}

	void VertexArray::Use() const
	{
		GLCall(glBindVertexArray(m_ID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Use();
		vb.Use();

		unsigned int offset = 0;
		const std::vector<VertexBufferLayoutElement> elements = layout.GetElements();

		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const VertexBufferLayoutElement element = elements[i];

			GLCall(glVertexAttribPointer(attrib_count + i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
			GLCall(glEnableVertexAttribArray(attrib_count + i));
			if (element.divisor)
				GLCall(glVertexAttribDivisor(attrib_count + i, element.divisor));

			offset += VertexBufferLayoutElement::SizeOfType(element.type) * element.count;
		}

		attrib_count += elements.size();
	}
}
