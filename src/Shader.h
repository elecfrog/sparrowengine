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
	unsigned int ID = 0;
	
	std::filesystem::path m_filePath;
	
	// caching for uniforms
	mutable std::unordered_map<std::string, int> m_locationCache;

public:
	enum ShaderType
	{
		NONE = 0,
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER
	};

	// Constructor generates the Shader Class on the fly
	// ------------------------------------------------------------------------
	Shader() = default;

	// Read Single File
	Shader(const std::filesystem::path& filepath);

	// Read Multiple Files
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		// Get Shader Strings
		std::string vertexCode = ParseShader(ShaderType::VERTEX, vertexPath);
		std::string fragmentCode = ParseShader(ShaderType::FRAGMENT, fragmentPath);

		// Create Shader Program
		CreateShaderProgram(vertexCode, fragmentCode);
	}

	std::string ParseShader(ShaderType shaderType, const std::filesystem::path& file_path)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string shaderCode;
		std::ifstream curr_file;
		// ensure ifstream objects can throw exceptions:
		curr_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			curr_file.open(file_path);
			std::stringstream file_stream;
			// read file's buffer contents into streams
			file_stream << curr_file.rdbuf();
			// close file handlers
			curr_file.close();
			// convert stream into string
			shaderCode = file_stream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::MyShader::" << shaderType << "FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}

		return shaderCode;
	}

	void CreateShaderProgram(const std::string& vertexCode, const std::string& fragmentCode)
	{

		unsigned int vertex = CompileShader2(ShaderType::VERTEX, vertexCode);
		unsigned int fragment = CompileShader2(ShaderType::FRAGMENT, fragmentCode);

		this->ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	unsigned int CompileShader2(ShaderType shaderType, const std::string& source)
	{
		unsigned int shader;

		const char* src = source.c_str();
		shader = glCreateShader(shaderType);

		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		checkCompileErrors2(shader, shaderType);

		return shader;
	}
	void checkCompileErrors2(unsigned int MyShader, ShaderType shaderType)
	{
		int success;
		char infoLog[1024];
		if (shaderType != ShaderType::NONE)
		{
			glGetShaderiv(MyShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(MyShader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

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
	
	void SetUniform3f(const std::string& name, glm::vec3 _vec);
	void SetUniform3f(const std::string& name, float _x, float _y, float _z);

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

	// utility function for checking MyShader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int MyShader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(MyShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(MyShader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(MyShader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(MyShader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};


#endif