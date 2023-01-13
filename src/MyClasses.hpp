#ifndef MY_H
#define MY_H

#include <filesystem>
#include <unordered_map>
#include <glm/glm.hpp>

class MyShader
{
private:
	unsigned int ID;

	// caching for uniforms
	mutable std::unordered_map<std::string, int> m_locationCache;

public:
	enum ShaderType
	{
		NONE = 0,
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER
	};

	// constructor generates the MyShader on the fly
	// ------------------------------------------------------------------------
	MyShader() = default;
	
	explicit MyShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{

		// Get Shader Strings
		std::string vertexCode   = ParseShader(ShaderType::VERTEX,vertexPath);
		std::string fragmentCode = ParseShader(ShaderType::FRAGMENT, fragmentPath);

		// Create Shader Program
		CreateShaderProgram(vertexCode, fragmentCode);
	}

	~MyShader()
	{
		glDeleteProgram(ID);
	}

	// activate the MyShader
	// ------------------------------------------------------------------------
	void Bind()
	{
		glUseProgram(ID);
	}

	void Unbind()
	{
		glUseProgram(0);
	}


	// utility uniform functions
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetUniform1i(const std::string& name, int value)
	{
		(glUniform1i(GetUniformLocation(name), value));
	}

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}

	void SetUniform4f(const std::string& name, glm::vec4 _vec)
	{
		(glUniform4f(GetUniformLocation(name), _vec.x, _vec.y, _vec.z, _vec.w));
	}

	void SetUniform3f(const std::string& name, glm::vec3 _vec)
	{
		(glUniform3f(GetUniformLocation(name), _vec.x, _vec.y, _vec.z));
	}

	void SetUniformMat4f(const std::string& name, glm::mat4& matrix)
	{
		(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	void SetUniformVec1i(const std::string& name, std::vector<int>& vector)
	{
		(glUniform1iv(GetUniformLocation(name), GLsizei(vector.size()), &vector[0]));
	}

private:
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
	
	unsigned int CompileShader(ShaderType shaderType, const std::string& source)
	{
		unsigned int shader;

		const char* src = source.c_str();
		shader = glCreateShader(shaderType);

		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		checkCompileErrors2(shader, shaderType);

		return shader;
	}

	void CreateShaderProgram(const std::string& vertexCode, const std::string& fragmentCode)
	{

		unsigned int vertex = CompileShader(ShaderType::VERTEX, vertexCode);

		unsigned int fragment = CompileShader(ShaderType::FRAGMENT, fragmentCode);

		this->ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	int GetUniformLocation(const std::string& name) const
	{
		if (const auto cache = m_locationCache.find(name); cache != m_locationCache.end())
			return cache->second;

		int location;
		GLCall(location = glGetUniformLocation(ID, name.c_str()));
		m_locationCache[name] = location;

		if (location == -1) std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";

		return location;
	}
};


struct RVertex
{// basic rendering vertex definition
	glm::vec3 position;
	glm::vec3 normals;
};


struct BVertex
{// basic rendering vertex definition
	glm::vec3 position;
	glm::vec2 texCoord;
};

class VBO
{
private:
	unsigned int ID;
public:

	VBO() = default;

	/*
				float vertex_data[] = {
					// positions          // uv
					 10.0f,  10.0f, 0.0f,   1.0f, 1.0f, // top right
					 10.0f, -10.0f, 0.0f,   1.0f, 0.0f, // bottom right
					-10.0f, -10.0f, 0.0f,   0.0f, 0.0f, // bottom left
					-10.0f,  10.0f, 0.0f,   0.0f, 1.0f  // top left
				};
	*/
	VBO(const void* data, unsigned int size)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);

		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}	
	

	/*
				std::vector<float> vertex_data = {
					// positions          // uv
					 10.0f,  10.0f, 0.0f,   1.0f, 1.0f, // top right
					 10.0f, -10.0f, 0.0f,   1.0f, 0.0f, // bottom right
					-10.0f, -10.0f, 0.0f,   0.0f, 0.0f, // bottom left
					-10.0f,  10.0f, 0.0f,   0.0f, 1.0f  // top left
				};
	*/
	VBO(std::vector<float>& vertex_data)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);

		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);
	}

	/*
			std::vector<BVertex> vertices = {
				BVertex{glm::vec3( 10.0f,  10.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
				BVertex{glm::vec3( 10.0f, -10.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
				BVertex{glm::vec3(-10.0f, -10.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
				BVertex{glm::vec3(-10.0f,  10.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
			};
	*/
	VBO(std::vector<BVertex>& b_vertices)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);

		glBufferData(GL_ARRAY_BUFFER, b_vertices.size() * sizeof(BVertex), b_vertices.data(), GL_STATIC_DRAW);
	}

	~VBO()
	{
		glDeleteBuffers(1, &ID);
	}

	void Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
protected:

};


class IBO
{
private:
	unsigned int ID;
public:

	IBO() = default;

	IBO(const void* data, unsigned int size)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	IBO(std::vector<unsigned int>& indies)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indies.size() * sizeof(unsigned int), indies.data(), GL_STATIC_DRAW);
	}

	~IBO()
	{
		glDeleteBuffers(1, &ID);
	}

	void Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

};


class VAO
{
private:
	unsigned int ID;
public:
	VAO()
	{
		glGenVertexArrays(1, &ID);
		Bind();
	}

	~VAO()
	{
		glDeleteVertexArrays(1, &ID);
	}

	void Bind()
	{
		glBindVertexArray(ID);
	}

	void Unbind()
	{
		glBindVertexArray(0);
	}

	void LinkAttrib(VBO& VBO, unsigned int layout, unsigned int numComponents, unsigned int type, unsigned int stride, void* offset)
	{
		VBO.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}

};
#endif