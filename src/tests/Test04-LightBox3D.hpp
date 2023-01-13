#pragma once

#include "Test.hpp"
#include "Utility.hpp"
#include "Texture.h"
#include "MyClasses.hpp"
#include "Mesh.h"

#include <GL/glew.h>
#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace test
{
	class Test04_LightBox3D : public Test
	{
		// create a OpenGL Objects: VAO, VBO, ShaderProgram
		
		// Mesh for rendering
		Mesh cube_mesh;
		Mesh light_cube;
		Mesh plane;

		// Shaders for Shading
		MyShader* m_shader;
		MyShader* cube_shader;
		MyShader* lightBox_shader;

		Texture* m_tex0;
		Texture* brick;

		// MVP matrices
		MyCamera main_camera;
		float eyePos = -3.0f;

		glm::mat4 cube_model		= glm::mat4(1.0f);

		// LIGHT
		Light m_light;

		// IMGUI variables
		float rectColor[3] = { 1.0f, 1.0f, 1.0f };
		float lightColor[3] = { 1.0f, 1.0f, 1.0f };
		glm::vec3 lightPos = glm::vec3(0.f, 0.f, 0.f);

		bool is_wireFrame = false;
		float cube_translated[3] = { 0.0f, 0.0f, 0.0f };

	public:
		Test04_LightBox3D()
		{
			// Enable Depth Test for 3D.
			(glEnable(GL_DEPTH_TEST));
			
			// Init Meshes
			// Init Plane_Mesh
			std::vector<AttribVertex> vertices = {
				AttribVertex{glm::vec3( 10.0f,  10.0f, 0.0f),glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
				AttribVertex{glm::vec3( 10.0f, -10.0f, 0.0f),glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
				AttribVertex{glm::vec3(-10.0f, -10.0f, 0.0f),glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
				AttribVertex{glm::vec3(-10.0f,  10.0f, 0.0f),glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
			};

			std::vector<unsigned int> indices = {  // note that we start from 0!
				0, 1, 3,  // first Triangle
				1, 2, 3   // second Triangle
			};
			
			plane.vertices = vertices;
			plane.indices = indices;
			
			// Init cube_mesh;
			cube_mesh.ReadFromObjFile("res/OBJFiles/Cube.obj", "res/OBJFiles/Cube");

			// Init LightCube:
			light_cube.vertices = cube_mesh.vertices;
			light_cube.indices = cube_mesh.indices;
			light_cube.LocalPosition = cube_mesh.LocalPosition;
			light_cube.WorldPosition = cube_mesh.WorldPosition;

			cube_mesh.Translate(glm::vec3(0.0f, 1.0f, 0.0f));
			light_cube.Translate(glm::vec3(2.0f, 2.0f, 2.0f));

			plane.BuildMeshFilter();
			cube_mesh.BuildMeshFilter();
			light_cube.BuildMeshFilter();

			//std::cout << "Light Cube World Position"
			//	<< light_cube.WorldPosition.x << "  " 
			//	<< light_cube.WorldPosition.y << "  " 
			//	<< light_cube.WorldPosition.z << "  " 
			//	<< std::endl;


			// Init Shaders
			m_shader		= new MyShader("res/Shaders/Plane.vert", "res/Shaders/Plane.frag");
			cube_shader		= new MyShader("res/Shaders/Cube.vert", "res/Shaders/Cube.frag");
			lightBox_shader = new MyShader("res/Shaders/LightBox.vert", "res/Shaders/LightBox.frag");

			// Init Textures
			m_tex0 = new Texture("res/textures/elecfrog.jpg");
			brick = new Texture("res/textures/brick.png");

			// Init Camera Settings
			main_camera.position = glm::vec3(0.0f, 0.0f, eyePos);
			main_camera.model = glm::rotate(main_camera.model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			main_camera.view = glm::translate(main_camera.view, glm::vec3(0.0f, 0.0f, eyePos));
			main_camera.proj = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
		}

		~Test04_LightBox3D() {}

		void OnUpdate(float deltaTime = 0.0f) override {}

		void OnRender() override
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (is_wireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			main_camera.TranslateCamera(glm::vec3(0.0f, 0.0f, eyePos));

			{// Render Elecfrog Plane
				m_tex0->Bind(0);
				m_shader->Bind();
				m_shader->SetUniformMat4f("u_model", main_camera.model);
				m_shader->SetUniformMat4f("u_view", main_camera.view);
				m_shader->SetUniformMat4f("u_proj", main_camera.proj);
				m_shader->SetUniform3f("u_color", glm::vec3(rectColor[0], rectColor[1], rectColor[2]));
				m_shader->SetUniform1i("u_texture0", 0);

				plane.DrawCallBind();
				glDrawElements(GL_TRIANGLES, plane.indices.size(), GL_UNSIGNED_INT, (const void*)nullptr);
				plane.DrawCallUnBind();
				m_shader->Unbind();
			}

			
			{// Render Cube
				brick->Bind(1);
				cube_shader->Bind();

				glm::mat4 mat_model = glm::translate(glm::mat4(1.0f), glm::vec3(cube_translated[0], cube_translated[1], cube_translated[2]));

				cube_shader->SetUniformMat4f("u_VP", main_camera.proj * main_camera.view);
				cube_shader->SetUniformMat4f("u_M", mat_model);
				cube_shader->SetUniform3f("lightColor", glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
				cube_shader->SetUniform3f("lightPos", light_cube.WorldPosition); // Here is a fixed value at (2,2,2)
				cube_shader->SetUniform1f("k_ambient", m_light.k_ambient);
				cube_shader->SetUniform1f("k_diffuse", m_light.k_diffuse);
				cube_shader->SetUniform1f("k_specular", m_light.k_specular);
				cube_shader->SetUniform1i("specular_intensity", m_light.specular_intensity);
				cube_shader->SetUniform3f("viewPos", glm::vec3(0.0f, 0.0f, eyePos));
				
				cube_shader->SetUniform1i("texSlot_1", 1);
				
				cube_mesh.DrawCallBind();
				glDrawElements(GL_TRIANGLES, cube_mesh.indices.size(), GL_UNSIGNED_INT, (const void*)nullptr);
				cube_mesh.DrawCallUnBind();
				cube_shader->Unbind();
			}

			{// Render LightBox
				lightBox_shader->Bind();
				glm::mat4 mat_model = glm::mat4(1.0f);
				lightBox_shader->SetUniformMat4f("u_MVP", main_camera.proj * main_camera.view * mat_model);
				lightBox_shader->SetUniform3f("lightColor", glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
				light_cube.DrawCallBind();
				glDrawElements(GL_TRIANGLES, light_cube.indices.size(), GL_UNSIGNED_INT, (const void*)nullptr);
				light_cube.DrawCallUnBind();
				lightBox_shader->Unbind();
			}
		}


		void OnImGuiRender() override 
		{ 
			ImGui::SliderFloat3("Rectangle Color", rectColor, 0.0f, 1.0f);
			ImGui::SliderFloat("Eye Position", &eyePos, -100.0f, 0.0f);
			ImGui::SliderFloat("Ambient Coefficient", &m_light.k_ambient, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Coefficient", &m_light.k_diffuse, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular Coefficient", &m_light.k_specular, 0.0f, 1.0f);
			ImGui::SliderInt("Specular Intensity", &m_light.specular_intensity, 0, 100);
			ImGui::SliderFloat3("Cube Position", cube_translated, -5.0f, 5.0f);
			ImGui::SliderFloat3("Light Color", lightColor, 0.0f, 1.0f);
			ImGui::Checkbox("WireFrame", &is_wireFrame);
		}
	};

}
