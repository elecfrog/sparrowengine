#pragma once

#include "Test.hpp"
#include "Utility.hpp"

#include <GL/glew.h>
#include <imgui/imgui.h>

#include "Shader.h"
#include "Texture.h"

// Include GLM for Math computing
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace test
{

	class Scene04_RollingCubes : public Test
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

        glm::vec3 cubePositions[10] = {
                glm::vec3( 0.0f,  0.0f,  0.0f),
                glm::vec3( 2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3( 2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3( 1.3f, -2.0f, -2.5f),
                glm::vec3( 1.5f,  2.0f, -2.5f),
                glm::vec3( 1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
        };

	public:
        Scene04_RollingCubes()
		{
            glEnable(GL_DEPTH_TEST);
            float rectangleVertices[] = {
				// positions                            // texture coords
                    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
            };

			unsigned int rectangleIndices[] = {  // note that we start from 0!
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};



			{// Rectangle Here
				GLCall(glGenVertexArrays(1, &rectVAO));

				GLCall(glGenBuffers(1, &rectVBO));
//				GLCall(glGenBuffers(1, &rectIBO));
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
//				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectIBO));
//				GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW));


				// Linking Vertex Attributes
				GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0));
				GLCall(glEnableVertexAttribArray(0));
//				GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float))));
//				GLCall(glEnableVertexAttribArray(1));
				GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float))));
				GLCall(glEnableVertexAttribArray(1));
			}

			m_Shader = new Shader("res/Shaders/Scene04/Scene04.vert", "res/Shaders/Scene04/Scene04.frag");

			m_Texture = new Texture("res/textures/container.jpg");
			m_TextureFrog = new Texture("res/textures/elecfrog.jpg");
			m_Texture->Bind(0);
			m_TextureFrog->Bind(1);
		}

		~Scene04_RollingCubes() {}

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
				m_Shader = new Shader("res/Shaders/Scene04/Scene04.vert", "res/Shaders/Scene04/Scene04.frag");
				reloadShaders = false;
			}
			
			m_Shader->Bind();
			m_Shader->SetUniform3f("editColorRGB", triColor[0], triColor[1], triColor[2]);
			m_Shader->SetUniform1i("container", 0);
			m_Shader->SetUniform1i("elecfrog", 1);


            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view,glm::vec3(0.0f,0,-3.0f));
            m_Shader->SetUniformMat4f("V", view);

            glm::mat4 proj = glm::mat4(1.0f);
            proj = glm::perspective(glm::radians(45.0f),16.0f/9.0f,0.1f,100.0f);
            m_Shader->SetUniformMat4f("P", proj);

			glBindVertexArray(rectVAO);
//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)nullptr);

            for(unsigned int i = 0; i < 10; ++i)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * (i+1);
                model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
                m_Shader->SetUniformMat4f("M", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }


//			glDrawArrays(GL_TRIANGLES, 0, 36);

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
