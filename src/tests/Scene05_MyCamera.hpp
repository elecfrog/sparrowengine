#pragma once

#include "Test.hpp"
#include "Utility.hpp"

#include <GL/glew.h>
#include <imgui/imgui.h>

#include "Shader.h"
#include "Texture.h"
#include "common/Camera.h"

// Include GLM for Math computing
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern GLFWwindow* window;

namespace test
{
	class Scene05_MyCamera : public Test
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

        // Camera Relevant
//        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//        // Direction Vector (Reversed)
//        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
//        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//        // Right Vector
//        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//        // Up Vector
//        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

//        glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
//        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//        glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
//
//        float yaw = 270.0f;
//        float pitch = 0.0f;
//        float FOV = 45.0f;
//        float keySensitivity = 1.0f;

        Camera m_camera;

	public:
        Scene05_MyCamera()
		{
            glEnable(GL_DEPTH_TEST);
            float rectangleVertices[] = {
				// positions                            // texture coords
                    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,      0.0f, 0.0f,
                    0.5f, -0.5f, -0.5f,     0.0f, 0.0f, -1.0f,      1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,     0.0f, 0.0f, -1.0f,      1.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,     0.0f, 0.0f, -1.0f,      1.0f, 1.0f,
                    -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,       0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,       0.0f, 0.0f,

                    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
                    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

                    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

                    -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,      0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,      0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,      0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
                    -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     0.0f, 0.0f,
                    -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     0.0f, 1.0f,

                    -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,  0.0f,    0.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,         0.0f, 1.0f,  0.0f,   1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,         0.0f, 1.0f,  0.0f,   1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,         0.0f, 1.0f,  0.0f,   1.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,        0.0f, 1.0f,  0.0f,    0.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,  0.0f,    0.0f, 1.0f
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
				GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0));
				GLCall(glEnableVertexAttribArray(0));
				GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float))));
				GLCall(glEnableVertexAttribArray(1));
				GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float))));
				GLCall(glEnableVertexAttribArray(2));
			}

			m_Shader = new Shader("res/Shaders/Scene05/Scene05.vert", "res/Shaders/Scene05/Scene05.frag");

			m_Texture = new Texture("res/textures/container.jpg");
			m_TextureFrog = new Texture("res/textures/elecfrog.jpg");
			m_Texture->Bind(0);
			m_TextureFrog->Bind(1);
		}

		~Scene05_MyCamera() {}

		void OnUpdate(float _deltaTime = 0.0f) override
        {
            // glfwGetTime is called only once, the first time this function is called
            static double lastTime = glfwGetTime();

            // Compute time difference between current and last frame
            double currentTime = glfwGetTime();
            float deltaTime = float(currentTime - lastTime);
            processInput(window, deltaTime);

            m_camera.UpdateCameraMatrix();
        }

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
				m_Shader = new Shader("res/Shaders/Scene05/Scene05.vert", "res/Shaders/Scene05/Scene05.frag");
				reloadShaders = false;
			}
			
			m_Shader->Bind();
			m_Shader->SetUniform3f("editColorRGB", triColor[0], triColor[1], triColor[2]);
			m_Shader->SetUniform1i("container", 0);
			m_Shader->SetUniform1i("elecfrog", 1);


            glm::mat4 view = m_camera.GetViewMatrix();
            m_Shader->SetUniformMat4f("V", view);

            glm::mat4 proj = m_camera.GetProjMatrix();
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
			if (ImGui::Button("ReloadShader") || ImGui::IsKeyPressed('F'))
			{
				reloadShaders = true;
			}
			ImGui::ColorEdit3("Object Color", triColor);
			ImGui::SliderFloat("Sensitivity", &m_camera.keySensitivity, 0.01f, 2.0f);
			ImGui::SliderFloat("Yaw Angle", &m_camera.yaw, 0.0f, 360.0f);
			ImGui::SliderFloat("Pitch Angle", &m_camera.pitch, -90.0f, 90.0f);
			ImGui::SliderFloat("FOV", &m_camera.FOV, 10.0f, 120.0f);

            ImGui::Text("W/A/S/D to control Camera Move");
            ImGui::Text("Q/E to control Camera Yaw");
            ImGui::Text("Ctrl+Q/E to control Camera Pitch");
            ImGui::Text("Alt+Q/E to control Camera FOV");
		}

        void processInput(GLFWwindow *window, float deltaTime)
        {
            float cameraSpeed = 0.05f * deltaTime; // adjust accordingly
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                m_camera.cameraPos += m_camera.keySensitivity * cameraSpeed * m_camera.cameraFront;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                m_camera.cameraPos -= m_camera.keySensitivity * cameraSpeed * m_camera.cameraFront;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                m_camera.cameraPos -= m_camera.keySensitivity * glm::normalize(glm::cross(m_camera.cameraFront, m_camera.cameraUp)) * cameraSpeed;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                m_camera.cameraPos += m_camera.keySensitivity * glm::normalize(glm::cross(m_camera.cameraFront, m_camera.cameraUp)) * cameraSpeed;
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
                m_camera.yaw -= m_camera.keySensitivity * 5.0f;
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
                m_camera.yaw += m_camera.keySensitivity * 5.0f;
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                m_camera.pitch -= m_camera.keySensitivity * 5.0f;
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                m_camera.pitch += m_camera.keySensitivity * 5.0f;
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
                m_camera.FOV -= m_camera.keySensitivity * 2.0f;
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
                m_camera.FOV += m_camera.keySensitivity * 2.0f;
        }

	};

}


