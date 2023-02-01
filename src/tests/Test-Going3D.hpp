#pragma once

#include "Test.hpp"
#include "Utility.hpp"

#include "Renderer.hpp"
#include "VertexBuffer.h"
#include "VertexBufferLayout.hpp"
#include "Texture.h"
#include "IndexBuffer.h"
#include "common/Camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/gtc/type_ptr.hpp> //value_ptr

#include <memory>
#include <type_traits>


namespace test
{

	class TestGoing3D : public Test
	{
		std::unique_ptr<VertexArray> m_vao;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<Camera> m_camera;

		// add texture
		std::unique_ptr<Texture> m_texture;
		//std::unique_ptr<Texture> texture_unit_2;

		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.0f));
		glm::mat4 m_model = glm::mat4(1.0f);
		glm::vec3 m_translationA = glm::vec3(200, 200, 0);
		glm::vec3 m_translationB = glm::vec3(400, 200, 0);

		Renderer m_renderer;

		float rotation = 0.0f;

		double prevTime = glfwGetTime();

	public:
		~TestGoing3D() {}
		TestGoing3D()
		{
			// Vertices coordinates
			float vertices[] =
			{ //     COORDINATES     /        COLORS      /   TexCoord  //
				-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
				-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
				 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
				 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
			};

			// Indices for vertices order
			unsigned int indices[] =
			{
				0, 1, 2,
				0, 2, 3,
				0, 1, 4,
				1, 2, 4,
				2, 3, 4,
				3, 0, 4
			};

			// Activate Blending to Keep rendering correct
			GLCall(glEnable(GL_DEPTH_TEST));
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			m_vao = std::make_unique<VertexArray>();

			static_assert(sizeof(float) == sizeof(std::remove_extent_t<decltype(vertices)>));
			m_vertexBuffer = std::make_unique<VertexBuffer>(vertices, unsigned(5 * 8 * sizeof(std::remove_extent_t<decltype(vertices)>)));
			VertexBufferLayout layout;

			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);

			m_vao->AddBuffer(*m_vertexBuffer, layout);

			m_indexBuffer = std::make_unique<IndexBuffer>(indices, 18);

			m_texture = std::make_unique<Texture>("res/textures/brick.png");

			m_shader  = std::make_unique<Shader>("res/shaders/Going3D.shader");

			m_camera  = std::make_unique<Camera>();
            m_camera->UpdateVPMatrices();
			//m_shader->Bind();
			//m_shader->SetUniform1i("u_Texture", 0);

		}

		void OnUpdate(float deltaTime = 0.0f) override {}

		void OnRender() override
		{
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			// Simple timer
			double crntTime = glfwGetTime();
			if (crntTime - prevTime >= 1 / 60)
			{
				rotation += 0.5f;
				prevTime = crntTime;


			}

			// Handles camera inputs
			//m_camera->Inputs(current_window);

			
			m_texture->Bind(0);
			{	// render the first texture
				glm::mat4 model = glm::rotate(m_model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));


				m_proj = m_camera->GetProjMatrix();
				m_view = m_camera->GetViewMatrix();

				glm::mat4 mvp = m_proj * m_view * model;

				m_shader->Bind();
				m_shader->SetUniformMat4f("u_MVP", mvp);
				m_shader->SetUniform1i("u_Texture", 0);

				m_renderer.Draw(*m_vao, *m_indexBuffer, *m_shader);
			}
		}

		void OnImGuiRender() override
		{
			ImGui::Text("Going 3D");
			
			ImGui::Text("Using Keyboard WASD to control the camera");

//			// Handles key inputs
//			if (ImGui::IsKeyPressed('W'))
//			{
//				m_camera->Position += m_camera->speed * m_camera->Orientation;
//			}
//			if (ImGui::IsKeyPressed('A'))
//			{
//				m_camera->Position += m_camera->speed * -glm::normalize(glm::cross(m_camera->Orientation, m_camera->Up));;
//			}
//			if (ImGui::IsKeyPressed('S'))
//			{
//				m_camera->Position += m_camera->speed * -m_camera->Orientation;
//			}
//			if (ImGui::IsKeyPressed('D'))
//			{
//				m_camera->Position += m_camera->speed * glm::normalize(glm::cross(m_camera->Orientation, m_camera->Up));
//			}
//			if (ImGui::IsKeyPressed(ImGuiKey_Space))
//			{
//				m_camera->Position += m_camera->speed * m_camera->Up;
//			}
//			if (ImGui::IsKeyPressed(ImGuiKey_LeftCtrl))
//			{
//				m_camera->Position += m_camera->speed * -m_camera->Up;
//			}
//			if (ImGui::IsKeyPressed(ImGuiKey_LeftShift))
//			{
//				m_camera->speed = 0.4f;
//			}
//			else if (ImGui::IsKeyReleased(ImGuiKey_LeftShift))
//			{
//				m_camera->speed = 0.1f;
//			}


			//// Handles mouse inputs
			//if (ImGui::GetMouseDragDelta() glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			//{
			//	// Hides mouse cursor
			//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			//	// Prevents camera from jumping on the first click
			//	if (firstClick)
			//	{
			//		glfwSetCursorPos(window, (width / 2), (height / 2));
			//		firstClick = false;
			//	}

			//	// Stores the coordinates of the cursor
			//	double mouseX;
			//	double mouseY;
			//	// Fetches the coordinates of the cursor
			//	glfwGetCursorPos(window, &mouseX, &mouseY);

			//	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			//	// and then "transforms" them into degrees 
			//	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
			//	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

			//	// Calculates upcoming vertical change in the Orientation
			//	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

			//	// Decides whether or not the next vertical Orientation is legal or not
			//	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			//	{
			//		Orientation = newOrientation;
			//	}

			//	// Rotates the Orientation left and right
			//	Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

			//	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			//	glfwSetCursorPos(window, (width / 2), (height / 2));
			//}
			//else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			//{
			//	// Unhides cursor since camera is not looking around anymore
			//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//	// Makes sure the next time the camera looks around it doesn't jump
			//	firstClick = true;
			//}
		}
	};

}
