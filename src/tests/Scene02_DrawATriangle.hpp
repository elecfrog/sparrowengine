#pragma once

#include "Test.hpp"
#include "Utility.hpp"

#include <GL/glew.h>
#include <imgui/imgui.h>

namespace test
{

	class Scene02_DrawATriangle : public Test
	{
		// create a OpenGL Objects: VAO, VBO, ShaderProgram
		unsigned int VAO;
		unsigned int VBO;
		unsigned int IBO;

		unsigned int rectVAO;
		unsigned int rectVBO;
		unsigned int rectIBO;

		unsigned int shader_program;

		bool drawATriangle = true;
		struct DrawShape
		{
			bool Triangle;
			bool Rectangle;
			bool TwoTriangles;
		}shapes;
		
		bool isWireFrame = false;

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
			"uniform vec3 editColorRGB;\n"
			"void main()\n"
			"{\n"
			"   color = vec4(editColorRGB.x, editColorRGB.y, editColorRGB.z, 1.0);\n"
			"}\0";
		
		float triColor[3] = { 50.0f / 255.0f,50.0f / 255.0f,50.0f / 255.0f };

	public:
		Scene02_DrawATriangle()
		{
			float triangleVertices[] = {
				// vec3::triangleVertices
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};

			unsigned int triangleIndices[] = {  // note that we start from 0!
				0, 1, 2,  // first Triangle
			};

			float rectangleVertices[] = {
				 0.5f,  0.5f, 0.0f,  // top right
				 0.5f, -0.5f, 0.0f,  // bottom right
				-0.5f, -0.5f, 0.0f,  // bottom left
				-0.5f,  0.5f, 0.0f   // top left 
			};

			unsigned int rectangleIndices[] = {  // note that we start from 0!
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};

			{// Triangle Here
				GLCall(glGenVertexArrays(1, &VAO));

				GLCall(glGenBuffers(1, &VBO));
				GLCall(glGenBuffers(1, &IBO));
				GLCall(glBindVertexArray(VAO));

				/* 
				*  GLenum target: which buffer is bound.
				*  GL_ARRAY_BUFFER: Vertex Attributes
				*/
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
				/*
				*  GLenum target: buffer bound in glBindBuffer().
				*  @(GLsizei size): Specifies the name of a buffer object
				*  TODO: Try revise this sentence.
				*/
				GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW));


				// bind IBO here
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
				GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW));


				// Linking Vertex Attributes
				GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0));
				GLCall(glEnableVertexAttribArray(0));
			}

			{// Rectangle Here
				GLCall(glGenVertexArrays(1, &rectVAO));

				GLCall(glGenBuffers(1, &rectVBO));
				GLCall(glGenBuffers(1, &rectIBO));
				GLCall(glBindVertexArray(rectVAO));

				/*
				*  GLenum target: which buffer is bound.
				*  GL_ARRAY_BUFFER: Vertex Attributes
				*/
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, rectVBO));
				/*
				*  GLenum target: buffer bound in glBindBuffer().
				*  @(GLsizei size): Specifies the name of a buffer object
				*  TODO: Try revise this sentence.
				*/
				GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW));


				// bind IBO here
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectIBO));
				GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW));


				// Linking Vertex Attributes
				GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0));
				GLCall(glEnableVertexAttribArray(0));
			}

			shapes = {1,0,0};



			unsigned int vertex_shader;
			GLCall(vertex_shader = glCreateShader(GL_VERTEX_SHADER));
			// TODO: why length is nullptr?
			GLCall(glShaderSource(vertex_shader, 1, &vertexShaderSource, nullptr));
			GLCall(glCompileShader(vertex_shader));
			int success;
			char infoLog[512];
			GLCall(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
			if (!success)
			{
				GLCall(glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog));
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			unsigned int frag_shader;
			GLCall(frag_shader = glCreateShader(GL_FRAGMENT_SHADER));
			// TODO: why length is nullptr?
			GLCall(glShaderSource(frag_shader, 1, &fragmentShaderSource, nullptr));
			GLCall(glCompileShader(frag_shader));
			GLCall(glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success));
			if (!success)
			{
				GLCall(glGetShaderInfoLog(frag_shader, 512, nullptr, infoLog));
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			//unsigned int shader_program;
			GLCall(shader_program = glCreateProgram());

			GLCall(glAttachShader(shader_program, vertex_shader));
			GLCall(glAttachShader(shader_program, frag_shader));
			GLCall(glLinkProgram(shader_program));
			// check for linking errors
			GLCall(glGetProgramiv(shader_program, GL_LINK_STATUS, &success));
			if (!success) {
				GLCall(glGetProgramInfoLog(shader_program, 512, NULL, infoLog));
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			}
			
			//glUseProgram(shader_program);
			GLCall(glDeleteShader(vertex_shader));
			GLCall(glDeleteShader(frag_shader));
		}

		~Scene02_DrawATriangle() {}

		void OnUpdate(float deltaTime = 0.0f) override {}

		void OnRender() override
		{
			if (isWireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(shader_program);

			int uniLocation;
			GLCall(uniLocation = glGetUniformLocation(shader_program, "editColorRGB"));
			glUniform3f(uniLocation, triColor[0], triColor[1], triColor[2]);

			if (shapes.Triangle)
			{
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const void*)nullptr);
			}
			else if(shapes.Rectangle)
			{
				glBindVertexArray(rectVAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)nullptr);

			}

		}

		void OnImGuiRender() override 
		{ 
			ImGui::Checkbox("Wireframe Mode", &isWireFrame);
			if (ImGui::RadioButton("Draw A Triangle", shapes.Triangle))
			{
				shapes.Triangle = true;
				shapes.Rectangle = false;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Draw A Rectangle", shapes.Rectangle))
			{
				shapes.Triangle = false;
				shapes.Rectangle = true;
			}
			ImGui::ColorEdit3("Triangle Color", triColor);
		}
	};

}
