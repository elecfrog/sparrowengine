#pragma once

#include "Test.hpp"
#include "Utility.hpp"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"
#include "Shader.h"

#include <GL/glew.h>
#include <imgui/imgui.h>

namespace test
{

	class Test01_DrawATriangle : public Test
	{
		// create a OpenGL Objects: VAO, VBO, ShaderProgram
		unsigned int VAO;
		unsigned int VBO;
		unsigned int IBO;
		unsigned int shader_program;

		Renderer renderer;

		const char* vertexShaderSource = 
			"#version 460 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		
		const char* fragmentShaderSource = 
			"#version 460 core\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"   color = vec4(1.0f, 0.0f, 0.0f, 1.0);\n"
			"}\0";

	public:
		Test01_DrawATriangle()
		{
			float vertex_data[] = {
				// vec3::vertex_data
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};

			unsigned int indices[] = {  // note that we start from 0!
				0, 1, 2,  // first Triangle
			};


			glGenVertexArrays(1, &VAO);

			glGenBuffers(1, &VBO);
			glGenBuffers(1, &IBO);
			glBindVertexArray(VAO);

			/* 
			*  GLenum target: which buffer is bound.
			*  GL_ARRAY_BUFFER: Vertex Attributes
			*/
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			/*
			*  GLenum target: buffer bound in glBindBuffer().
			*  @(GLsizei size): Specifies the name of a buffer object
			*  TODO: Try revise this sentence.
			*/
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);


			// bind IBO here
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


			// Linking Vertex Attributes
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
			glEnableVertexAttribArray(0);



			unsigned int vertex_shader;
			vertex_shader = glCreateShader(GL_VERTEX_SHADER);
			// TODO: why length is nullptr?
			glShaderSource(vertex_shader, 1, &vertexShaderSource, nullptr);
			glCompileShader(vertex_shader);
			int success;
			char infoLog[512];
			glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			unsigned int frag_shader;
			frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
			// TODO: why length is nullptr?
			glShaderSource(frag_shader, 1, &fragmentShaderSource, nullptr);
			glCompileShader(frag_shader);
			glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(frag_shader, 512, nullptr, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			//unsigned int shader_program;
			shader_program = glCreateProgram();

			glAttachShader(shader_program, vertex_shader);
			glAttachShader(shader_program, frag_shader);
			glLinkProgram(shader_program);
			// check for linking errors
			glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
			
			//glUseProgram(shader_program);
			glDeleteShader(vertex_shader);
			glDeleteShader(frag_shader);
		}

		~Test01_DrawATriangle() {}

		void OnUpdate(float deltaTime = 0.0f) override {}

		void OnRender() override
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(shader_program);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const void*)nullptr);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		void OnImGuiRender() override { ImGui::Text("Test01 Draw A triangle"); }
	};

}
