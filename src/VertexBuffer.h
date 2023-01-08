#ifndef VERTEXBUFFER_CLASS_H
#define VERTEXBUFFER_CLASS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


#include "Utility.hpp"

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VertexBuffer
{
private:
	// Reference ID of the Vertex Buffer Object
	unsigned int m_rendererId;

public:
	
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VertexBuffer(const void* data, unsigned int size);

	//VBO(std::vector<Vertex>& vertices);

	// Deletes the VBO
	~VertexBuffer();

	// Binds the VBO
	void Bind() const;

	// Unbinds the VBO
	void Unbind() const;
};

#endif