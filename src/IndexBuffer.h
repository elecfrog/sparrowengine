#ifndef INDEXBUFFER_CLASS_H
#define INDEXBUFFER_CLASS_H

#include <GL/glew.h>
#include "Utility.hpp"

class IndexBuffer
{
private:
	unsigned int m_rendererId;
	unsigned int m_count;

public:
	// data -- the pointer points to the indices array
	// count -- size of the indices array
	IndexBuffer();

	// Constructor that generates a Elements Buffer Object and links it to indices
	IndexBuffer(const unsigned int* data, unsigned int count);

	//EBO::EBO(std::vector<GLuint>& indices)
	//{
	//	glGenBuffers(1, &ID);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	//}

	~IndexBuffer();

	// Binds the IndexBuffer
	void Bind() const;

	// Unbinds the IndexBuffer
	void Unbind() const;

	unsigned int GetCount() const;
};

#endif