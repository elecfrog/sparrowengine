#include "Shader.h"

Shader::Shader(const std::filesystem::path& filepath)
		: m_filePath(filepath)
	{
		ShaderProgramSource source = ParseShader(m_filePath);
		ID = CreateShader(source.vertexSource, source.fragmentSource);
	}

Shader::~Shader()
	{
		GLCall(glDeleteProgram(ID));
	}

void Shader::Bind() const
	{
		GLCall(glUseProgram(ID));
	}

void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

/***********************************************
* Utility functions to help to set uniform values
************************************************/
void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 _vec)
{
	GLCall(glUniform4f(GetUniformLocation(name), _vec.x, _vec.y, _vec.z, _vec.w));
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 _vec)
{
	GLCall(glUniform3f(GetUniformLocation(name), _vec.x, _vec.y, _vec.z));
}

void Shader::SetUniform3f(const std::string& name, float _x, float _y, float _z)
{
	GLCall(glUniform3f(GetUniformLocation(name), _x, _y, _z));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformVec1i(const std::string& name, std::vector<int>& vector)
{
	GLCall(glUniform1iv(GetUniformLocation(name), GLsizei(vector.size()), &vector[0]));
}

/***********************************************
* Parse Shader Text File from the Hard Disk to Vertex Shader Strings and Fragment Shader Strings
************************************************/
ShaderProgramSource Shader::ParseShader(const std::filesystem::path& filePath)
	{
		enum class ShaderType { none = -1, vertex = 0, fragment = 1 };

		std::ifstream stream(filePath);
		if (!stream)
			exit(EXIT_FAILURE);
		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::none;

		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::vertex;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::fragment;
			}
			else
			{
				ss[static_cast<int>(type)] << line << '\n';
			}
		}
		return { ss[0].str(), ss[1].str() };
	}

/***********************************************
* Compile Different Kinds of Shader Strings
************************************************/
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader;
	GLCall(shader = glCreateShader(type));

	const char* src = source.c_str();
	GLCall(glShaderSource(shader, 1, &src, nullptr));
	GLCall(glCompileShader(shader));

	int result;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		GLCall(glGetShaderInfoLog(shader, length, nullptr, message));

		std::cerr << "Error: Fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
		std::cerr << message << std::endl;

		GLCall(glDeleteShader(shader));
		return 0;
	}

	return shader;
}

/***********************************************
* Create Shader Program
************************************************/
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program;
	GLCall(program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (const auto cache = m_locationCache.find(name); cache != m_locationCache.end())
		return cache->second;

	int location;
	GLCall(location = glGetUniformLocation(ID, name.c_str()));
	m_locationCache[name] = location;

	if (location == -1) std::cerr << "Warning: uniform '" << name << "' doesn't exist!\n";

	return location;
}