#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3> _vertices, 
	//std::vector<glm::vec4> _colorsRGBA,
	std::vector<glm::vec2> _textureCoordinates, 
	std::vector<glm::vec3> _normals, 
	std::vector<unsigned int> _faces)
{
	this->vertices = _vertices;
	//this->colorsRGBA = _colorsRGBA;
	this->textureCoordinates = _textureCoordinates;
	this->_normals = normals;
	this->_faces = faces;
}

std::vector<glm::vec3> Mesh::GetVertices() const
{
	return vertices;
}

//std::vector<glm::vec4> Mesh::GetColors() const
//{
//	return colorsRGBA;
//}

std::vector<glm::vec2> Mesh::GetTextCoordiantes() const
{
	return textureCoordinates;
}

std::vector<glm::vec3> Mesh::GetNormals() const
{
	return normals;
}

std::vector<unsigned int> Mesh::GetFaces() const
{
	return faces;
}

void Mesh::GenAttriMesh()
{
	if (IsEqual() && vertices != nullptr)
	{
		for (unsigned int v = 0; v < vertices.size(); ++v)
		{
			AttriVertex currentVertex;
			currentVertex.position  = vertices[v];
			//currentVertex.colorRGBA = colorsRGBA[v];
			currentVertex.texCoord = textureCoordinates[v];
			currentVertex.normals = normals[v];

			attriMesh.emplace_back(currentVertex);
		}

	}
}

bool Mesh::IsEqual()
{
	if (vertices.size() /*== colorsRGBA.size()*/ == textureCoordinates.size() == normals.size())
		return true;
	return false;
}
