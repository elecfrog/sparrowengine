#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "Utility.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

struct AttriVertex
{
	glm::vec3 position;
	//glm::vec4 colorRGBA;
	glm::vec2 texCoord;
	glm::vec3 normals;
};

class Mesh
{

private:
	std::vector<glm::vec3> vertices;
	
	//std::vector<glm::vec4> colorsRGBA;
	
	std::vector<glm::vec2> textureCoordinates;
	
	std::vector<glm::vec3> normals;
	
	std::vector<unsigned int> faces;

public:
	Mesh() = default;

	Mesh(std::vector<glm::vec3> _vertices, 
		//std::vector<glm::vec4> _colorsRGBA,
		std::vector<glm::vec2> _textureCoordinates,
		std::vector<glm::vec3> _normals,
		std::vector<unsigned int> _faces);

	std::vector<AttriVertex> attriMesh;
	
	/***********************************************
	* Utility functions to help to set uniform values
	************************************************/
	std::vector<glm::vec3>		GetVertices() const ;
	//std::vector<glm::vec4>		GetColors() const;
	std::vector<glm::vec2>		GetTextCoordiantes() const;
	std::vector<glm::vec3>		GetNormals() const;
	std::vector<unsigned int>	GetFaces() const;

	void GenAttriMesh();

	void ReadFromModel();
	void WriteToModel();

private:
	bool IsEqual();
};


#endif // MESH_CLASS_H