#pragma once

#include "Test.hpp"
#include "Utility.hpp"

#include <GL/glew.h>
#include <imgui/imgui.h>

#include "Shader.h"
#include "Texture.h"

namespace test
{

	class Scene03_TextureRectangle : public Test
	{
		// create a OpenGL Objects: VAO, VBO, ShaderProgram
		unsigned int rectVAO;
		unsigned int rectVBO;
		unsigned int rectIBO;
		Shader* m_Shader;
		Texture* m_Texture;		
		Texture* m_TextureFrog;

		bool isWireFrame = false;
		bool reloadShaders = false;
		
		float triColor[3] = { 1.0f, 1.0f, 1.0f };

	public:
		Scene03_TextureRectangle()
		{
			float rectangleVertices[] = {
				// positions          // colors           // texture coords
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 			
			};

			unsigned int rectangleIndices[] = {  // note that we start from 0!
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};

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
				GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0));
				GLCall(glEnableVertexAttribArray(0));
				GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float))));
				GLCall(glEnableVertexAttribArray(1));
				GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float))));
				GLCall(glEnableVertexAttribArray(2));
			}

			m_Shader = new Shader("res/Shaders/Scene03/Scene03.vert", "res/Shaders/Scene03/Scene03.frag");

			m_Texture = new Texture("res/textures/container.jpg");
			m_TextureFrog = new Texture("res/textures/elecfrog.jpg");
			m_Texture->Bind(0);
			m_TextureFrog->Bind(1);
		}

		~Scene03_TextureRectangle() {}

		void OnUpdate(float deltaTime = 0.0f) override {}

		void OnRender() override
		{
			if (isWireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (reloadShaders)
			{
				m_Shader->~Shader();
				m_Shader = new Shader("res/Shaders/Scene03/Scene03.vert", "res/Shaders/Scene03/Scene03.frag");
				reloadShaders = false;
			}
			
			m_Shader->Bind();
			m_Shader->SetUniform3f("editColorRGB", triColor[0], triColor[1], triColor[2]);
			m_Shader->SetUniform1i("container", 0);
			m_Shader->SetUniform1i("elecfrog", 1);


			glBindVertexArray(rectVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)nullptr);

		}

		void OnImGuiRender() override 
		{ 
			ImGui::Checkbox("Wireframe Mode", &isWireFrame);
			if (ImGui::Button("ReloadShader") || ImGui::IsKeyPressed('R'))
			{
				reloadShaders = true;
			}
			ImGui::ColorEdit3("Object Color", triColor);
		}
	};

}
