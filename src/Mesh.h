#ifndef m_CLASS_H
#define m_CLASS_H

#include "Utility.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

#include "tiny_obj_loader.h"

#include "MyClasses.hpp"

class Mesh
{

public:
	// expose for accessing
	std::vector<AttribVertex> vertices;
	
	std::vector<unsigned int> indices;

	glm::vec3 LocalPosition;
	
	glm::vec3 WorldPosition;


	VAO* m_VAO;
	VBO* m_VBO;
	IBO* m_IBO;
	
	//std::map<std::string, Shader*> m_shaders;
	MyShader* m_shader;

	glm::mat4 mat_Local; // transform itself in local space
	glm::mat4 mat_transformLocalToWorld; // transform itself in local space
	
	glm::mat4 mat_render;
	// TODO:
	/*
	*	1. Hold A Central Point
	*	2. Build A Bounding Box
	*	3. Hold A Model Matrix used to transform positions while update in rendering
	*	4. Hold A Shader? (depending on Unity)
	*	5. Hold A texture?
	*	6. Transform both from local & world.
	*/
public:
	Mesh();

	//Mesh(std::vector<glm::vec3> _vertices, 
	//	//std::vector<glm::vec4> _colorsRGBA,
	//	std::vector<glm::vec2> _textureCoordinates,
	//	std::vector<glm::vec3> _normals,
	//	std::vector<unsigned int> _faces);

	//std::vector<AttriVertex> attriMesh;
	
	/***********************************************
	* Utility functions to help to set uniform values
	************************************************/
	//std::vector<glm::vec3>		GetVertices() const ;
	//std::vector<glm::vec4>		GetColors() const;
	//std::vector<glm::vec2>		GetTextCoordiantes() const;
	//std::vector<glm::vec3>		GetNormals() const;
	//std::vector<unsigned int>	GetFaces() const;

	glm::mat4 GetRenderModel() const;
	void SetRenderModel(const glm::mat4& _model);
	void SetRenderModel(const glm::vec3& _translate);

	void Translate(const glm::vec3& _translate);

	void ReadFromObjFile(const char* _filePath, const char* _basePath);
	void BuildMeshFilter();

	void DrawCallBind();
	void DrawCallUnBind();

private:
	//bool IsEqual();
	void PrintIndices();
	void PrintVertices();
	void PrintCerntralPoint();
	void PrintSingle(AttribVertex v);
};


#endif // m_CLASS_H