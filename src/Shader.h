#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

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

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{

private:
	unsigned int m_RendererId = 0;
	
	std::filesystem::path m_filePath;
	
	// caching for uniforms
	mutable std::unordered_map<std::string, int> m_locationCache;

public:
	Shader(const std::filesystem::path& filepath);

	~Shader();

	// Activates the Shader Program
	void Bind() const;

	void Unbind() const;


	/***********************************************
	* Utility functions to help to set uniform values
	************************************************/
	void SetUniform1i(const std::string& name, int value);

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void SetUniform4f(const std::string& name, glm::vec4 _vec);
	
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);

	void SetUniformVec1i(const std::string& name, std::vector<int>& vector);

private:
	/***********************************************
	* Parse Shader Text File from the Hard Disk to Vertex Shader Strings and Fragment Shader Strings
	************************************************/
	ShaderProgramSource ParseShader(const std::filesystem::path& filePath);

	/***********************************************
	* Compile Different Kinds of Shader Strings
	************************************************/
	unsigned int CompileShader(unsigned int type, const std::string& source);

	/***********************************************
	* Create Shader Program
	************************************************/
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name) const;
};


#endif