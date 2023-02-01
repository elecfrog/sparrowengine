//#pragma once
//
//#include "Test.hpp"
//#include "Utility.hpp"
//
//#include "Renderer.hpp"
//#include "VertexBuffer.h"
//#include "VertexBufferLayout.hpp"
//#include "Texture.h"
//#include "IndexBuffer.h"
//
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#include <imgui/imgui.h>
//#include <glm/glm.hpp>					//vec3, vec4, ivec4, mat4
//#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective
//#include <glm/gtc/type_ptr.hpp>			//value_ptr
//
//#include <memory>
//#include <type_traits>
//
//
//namespace test
//{
//
//	class Test10_SpecularMaps : public Test
//	{
//		// Pyramid Objects for rendering.
//		std::unique_ptr<VertexArray>	pyramid_vao;
//		std::unique_ptr<IndexBuffer>	pyramid_ibo;
//		std::unique_ptr<VertexBuffer>	pyramid_vbo;
//		std::unique_ptr<Shader>			pyramid_shader;
//
//		// LightBox Objects for rendering.
//		std::unique_ptr<VertexArray>	light_vao;
//		std::unique_ptr<IndexBuffer>	light_ibo;
//		std::unique_ptr<VertexBuffer>	light_vbo;
//		std::unique_ptr<Shader>			light_shader;
//
//		// TODO: Make a vec3 and RGB, RGBA, RGBValue(0-255) for colors.
//		float lightColorRGB[3] = { 1.0f, 1.0f, 1.0f };
//		float lightPos[3] = { 0.5f, 0.5f, 0.5f };
//
//		// MainCamera for Viewport rendering.
//		std::unique_ptr<Camera> mainCamera;
//
//		// add texture
//		std::unique_ptr<Texture> tex_woodPlane;
//		std::unique_ptr<Texture> tex_woodPlaneSpec;
//		//std::unique_ptr<Texture> texture_unit_2;
//
//		glm::mat4 m_proj = glm::mat4(1.0f);
//		glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -2.0f));
//		glm::mat4 m_model = glm::mat4(1.0f);
//		glm::vec3 m_translationA = glm::vec3(200, 200, 0);
//		glm::vec3 m_translationB = glm::vec3(400, 200, 0);
//
//		Renderer m_renderer;
//
//		float rotation = 0.0f;
//
//		double prevTime = glfwGetTime();
//
//	public:
//		~Test10_SpecularMaps() {}
//		Test10_SpecularMaps()
//		{
//			// Pyramid Vertices coordinates
//			float vertices[] =
//			{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
//				-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
//				-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
//				 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
//				 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
//
//				-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
//				-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
//				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side
//
//				-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
//				 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
//				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
//
//				 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
//				 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
//				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side
//
//				 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
//				-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
//				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
//			};
//
//			// Indices for vertices order
//			unsigned int indices[] =
//			{
//				0, 1, 2, // Bottom side
//				0, 2, 3, // Bottom side
//				4, 6, 5, // Left side
//				7, 9, 8, // Non-facing side
//				10, 12, 11, // Right side
//				13, 15, 14 // Facing side
//			};
//
//			// Light Box Vertices coordinates
//			float lightVertices[] =
//			{ //     COORDINATES     //
//				-0.1f, -0.1f,  0.1f,
//				-0.1f, -0.1f, -0.1f,
//				 0.1f, -0.1f, -0.1f,
//				 0.1f, -0.1f,  0.1f,
//				-0.1f,  0.1f,  0.1f,
//				-0.1f,  0.1f, -0.1f,
//				 0.1f,  0.1f, -0.1f,
//				 0.1f,  0.1f,  0.1f
//			};
//
//			unsigned int lightIndices[] =
//			{
//					0, 1, 2,
//					0, 2, 3,
//					0, 4, 7,
//					0, 7, 3,
//					3, 7, 6,
//					3, 6, 2,
//					2, 6, 5,
//					2, 5, 1,
//					1, 5, 4,
//					1, 4, 0,
//					4, 5, 6,
//					4, 6, 7
//			};
//
//
//			// Activate Blending to Keep rendering correct
//			GLCall(glEnable(GL_DEPTH_TEST));
//			GLCall(glEnable(GL_BLEND));
//			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//
//			{ // init pyramid object
//				pyramid_vao = std::make_unique<VertexArray>();
//				static_assert(sizeof(float) == sizeof(std::remove_extent_t<decltype(vertices)>));
//				pyramid_vbo = std::make_unique<VertexBuffer>(vertices, unsigned(176 * sizeof(std::remove_extent_t<decltype(vertices)>)));
//				VertexBufferLayout layout;
//
//				layout.Push<float>(3);
//				layout.Push<float>(3);
//				layout.Push<float>(2);
//				layout.Push<float>(3);
//
//				pyramid_vao->AddBuffer(*pyramid_vbo, layout);
//				pyramid_ibo = std::make_unique<IndexBuffer>(indices, 18);
//
//				// init different texture instances in memory
//				tex_woodPlane = std::make_unique<Texture>("res/textures/planks.png");
//				tex_woodPlaneSpec = std::make_unique<Texture>("res/textures/planksSpec.png");
//
//				pyramid_shader = std::make_unique<Shader>("res/shaders/Pyramid.shader");
//
//				pyramid_vao->Unbind();
//				pyramid_vbo->Unbind();
//				pyramid_ibo->Unbind();
//				pyramid_shader->Unbind();
//			}
//
//
//			{ // init light box object
//				light_vao = std::make_unique<VertexArray>();
//				static_assert(sizeof(float) == sizeof(std::remove_extent_t<decltype(lightVertices)>));
//				light_vbo = std::make_unique<VertexBuffer>(lightVertices, unsigned(24 * sizeof(std::remove_extent_t<decltype(lightVertices)>)));
//				VertexBufferLayout light_layout;
//
//				light_layout.Push<float>(3);
//
//				light_vao->AddBuffer(*light_vbo, light_layout);
//
//				light_ibo = std::make_unique<IndexBuffer>(lightIndices, sizeof(lightIndices));
//				light_shader = std::make_unique<Shader>("res/shaders/LightBox.shader");
//
//				light_vao->Unbind();
//				light_vbo->Unbind();
//				light_ibo->Unbind();
//				light_shader->Unbind();
//
//				// light attributes:
//			}
//
//			// init main camera right now.
//			mainCamera = std::make_unique<Camera>(800, 800, glm::vec3(0.0f, 0.0f, 5.0f));
//
//			//pyramid_shader->Bind();
//			//pyramid_shader->SetUniform1i("u_Texture", 0);
//
//		}
//
//		void OnUpdate(float deltaTime = 0.0f) override {}
//
//		void OnRender() override
//		{
//			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
//			GLCall(glClear(GL_COLOR_BUFFER_BIT));
//
//			// Simple timer
//			double crntTime = glfwGetTime();
//			if (crntTime - prevTime >= 1 / 60)
//			{
//				rotation += 0.5f;
//				prevTime = crntTime;
//
//			}
//			// Handles camera inputs
//			//mainCamera->Inputs(current_window);
//
//
//			// Rendering Attributes Setting:
//
//				/*
//				* Setting Model Transformations towards each object at first.
//				*/
//			glm::mat4 pyramid_model = glm::rotate(m_model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
//			glm::mat4 lightbox_model = glm::translate(m_model, glm::vec3(lightPos[0], lightPos[1], lightPos[2]));
//
//			glm::mat4 mvp = m_proj * m_view * pyramid_model;
//
//
//			/*
//			* Setting Camera.
//			*/
//			mainCamera->ProjMatrix(45.0f, 0.1f, 100.0f);
//			mainCamera->ViewMatrix(45.0f, 0.1f, 100.0f);
//			m_proj = mainCamera->ProjMatrix(45.0f, 0.1f, 100.0f);
//			m_view = mainCamera->ViewMatrix(45.0f, 0.1f, 100.0f);
//
//
//			/*
//			* Setting Light Attributes.
//			*/
//
//			glm::vec4 lightColor = glm::vec4(lightColorRGB[0], lightColorRGB[1], lightColorRGB[2], 1.0f);
//
//			{ // render the pyramid
//
//				// bind different textures for rendering
//				tex_woodPlane->Bind(0);
//				tex_woodPlaneSpec->Bind(1);
//
//				pyramid_shader->Bind();
//				pyramid_shader->SetUniformMat4f("u_MVP", mvp);
//
//				// Set for uniforms in the shaders
//				pyramid_shader->SetUniform1i("u_Texture0", 0);
//				pyramid_shader->SetUniform1i("u_Texture1", 1);
//
//				pyramid_shader->SetUniform4f("u_lightColor", lightColor);
//				pyramid_shader->SetUniformMat4f("u_Model", pyramid_model);
//				pyramid_shader->SetUniform3f("u_lightPos", glm::vec3(lightPos[0], lightPos[1], lightPos[2]));
//				pyramid_shader->SetUniform3f("u_cameraPos", glm::vec3(mainCamera->Position.x, mainCamera->Position.y, mainCamera->Position.z));
//
//				m_renderer.Draw(*pyramid_vao, *pyramid_ibo, *pyramid_shader);
//			}
//
//
//			{	// render light box.
//
//				mvp = m_proj * m_view * lightbox_model;
//				light_shader->Bind();
//				light_shader->SetUniformMat4f("u_MVP", mvp);
//				light_shader->SetUniform4f("u_lightColor", lightColor);
//				m_renderer.Draw(*light_vao, *light_ibo, *light_shader);
//			}
//		}
//
//		void OnImGuiRender() override
//		{
//			ImGui::Text("Lighting");
//
//			ImGui::Text("Using Keyboard W/S/A/D/CTRL/SPACE to control the camera");
//			ImGui::SliderFloat3("Light Color", lightColorRGB, 0.0f, 1.0f);
//			ImGui::SliderFloat3("Light Position", lightPos, 0.0f, 1.0f);
//			// Handles key inputs
//			if (ImGui::IsKeyPressed('W'))
//			{
//				mainCamera->Position += mainCamera->speed * mainCamera->Orientation;
//			}
//			if (ImGui::IsKeyPressed('A'))
//			{
//				mainCamera->Position += mainCamera->speed * -glm::normalize(glm::cross(mainCamera->Orientation, mainCamera->Up));;
//			}
//			if (ImGui::IsKeyPressed('S'))
//			{
//				mainCamera->Position += mainCamera->speed * -mainCamera->Orientation;
//			}
//			if (ImGui::IsKeyPressed('D'))
//			{
//				mainCamera->Position += mainCamera->speed * glm::normalize(glm::cross(mainCamera->Orientation, mainCamera->Up));
//			}
//			if (ImGui::IsKeyPressed(ImGuiKey_Space))
//			{
//				mainCamera->Position += mainCamera->speed * mainCamera->Up;
//			}
//			if (ImGui::IsKeyPressed(ImGuiKey_LeftCtrl))
//			{
//				mainCamera->Position += mainCamera->speed * -mainCamera->Up;
//			}
//			if (ImGui::IsKeyPressed(ImGuiKey_LeftShift))
//			{
//				mainCamera->speed = 0.4f;
//			}
//			else if (ImGui::IsKeyReleased(ImGuiKey_LeftShift))
//			{
//				mainCamera->speed = 0.1f;
//			}
//
//		}
//	};
//
//}
