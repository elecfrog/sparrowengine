#include "IndexBuffer.h"

// data -- the pointer points to the indices array
// count -- size of the indices array
IndexBuffer::IndexBuffer()
		:m_count(0), m_rendererId(0)
	{

	}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		: m_count(count)
	{
		ASSERT(sizeof(m_count) == sizeof(GLuint));

		GLCall(glGenBuffers(1, &m_rendererId));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
		GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW));
	}

IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_rendererId));
	}

void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
	}

void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

unsigned int IndexBuffer::GetCount() const
	{
		return m_count;
	}