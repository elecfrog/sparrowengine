#pragma once

#include "Test.hpp"
#include "Utility.hpp"
#include "Texture.h"
#include "MyClasses.hpp"

#include <GL/glew.h>
#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace test
{
	
	class Test03_Box3D : public Test
	{
		// create a OpenGL Objects: VAO, VBO, ShaderProgram
		VBO* m_VBO;
		IBO* m_IBO;
		VAO* m_VAO;
		MyShader* m_shader;
		Texture* m_tex0;

		float rectColor[3] = { 1.0f, 0.0f, 1.0f	};
		float eyePos = -3.0f;
		bool is_wireFrame = false;

		glm::mat4 model = glm::mat4(1.0f);
		
		glm::mat4 view = glm::mat4(1.0f);
		
		glm::mat4 proj;

		// Load A Cube Model
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn;
		std::string err;

		VBO* cube_VBO;
		IBO* cube_IBO;
		VAO* cube_VAO;
		MyShader* cube_shader;
		MyShader* lightBox_shader;
		//Texture* m_tex0;

		std::vector<DUVertex> cube_vertices;
		std::vector<unsigned int> cube_indices;

		float cube_translated[3] = { 0.0f, 1.5f, 0.0f };
		glm::mat4 cube_model = glm::mat4(1.0f);

	public:
		Test03_Box3D()
		{
			(glEnable(GL_DEPTH_TEST));


			std::vector<float> vertex_data = {
				// positions          // uv
				 10.0f,  10.0f, 0.0f,   1.0f, 1.0f, // top right
				 10.0f, -10.0f, 0.0f,   1.0f, 0.0f, // bottom right
				-10.0f, -10.0f, 0.0f,   0.0f, 0.0f, // bottom left
				-10.0f,  10.0f, 0.0f,   0.0f, 1.0f  // top left 
			};			
			
			std::vector<BVertex> vertices = {
				BVertex{glm::vec3( 10.0f,  10.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
				BVertex{glm::vec3( 10.0f, -10.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
				BVertex{glm::vec3(-10.0f, -10.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
				BVertex{glm::vec3(-10.0f,  10.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
			};

			std::vector<unsigned int> indices = {  // note that we start from 0!
				0, 1, 3,  // first Triangle
				1, 2, 3   // second Triangle
			};


			m_VAO = new VAO();
			m_VBO = new VBO(vertices);
			m_IBO = new IBO(indices);

			// Linking Vertex Attributes
			m_VAO->LinkAttrib(*m_VBO, 0, 3, GL_FLOAT, sizeof(BVertex), (void*)0);
			m_VAO->LinkAttrib(*m_VBO, 1, 2, GL_FLOAT, sizeof(BVertex), (void*)(3 * sizeof(float)));
			m_shader = new MyShader("res/Shaders/Test03-Box3D.vert.glsl", "res/Shaders/Test02_frag.glsl");

			m_tex0 = new Texture("res/textures/elecfrog.jpg");

			m_VAO->Unbind();
			m_VBO->Unbind();
			m_IBO->Unbind();
			m_shader->Unbind();

			bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "res/OBJFiles/Cube.obj", "res/OBJFiles", true);
			const auto& _vertices = attrib.vertices;
			const auto& _normals = attrib.normals;
			const auto& _texCoords = attrib.texcoords;
			
			for (unsigned int v = 0; v < attrib.vertices.size()/3; ++v)
			{
				DUVertex tmp_v = DUVertex{
					glm::vec3(_vertices[3 * v], _vertices[3 * v + 1], _vertices[3 * v + 2]),
					glm::vec3(_normals[3 * v], _normals[3 * v + 1], _normals[3 * v+2]),
					glm::vec2(_texCoords[2 * v], _texCoords[2 * v + 1])
				};
				cube_vertices.emplace_back(tmp_v);
			}
			// For each shape
			//printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(0), static_cast<unsigned long>(shapes[0].mesh.indices.size()));

			size_t index_offset = 0;
			//printf("shape[%ld].num_faces: %lu\n", static_cast<long>(0), static_cast<unsigned long>(shapes[0].mesh.num_face_vertices.size()));

			// For each face
			for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
				size_t fnum = shapes[0].mesh.num_face_vertices[f];

				// For each vertex in the face
				for (size_t v = 0; v < fnum; v++) {
					tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
					printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
						static_cast<long>(v), idx.vertex_index, idx.normal_index,
						idx.texcoord_index);
					cube_indices.emplace_back(idx.vertex_index);
				}

				index_offset += fnum;
			}

			for (unsigned int v = 0; v < cube_indices.size(); ++v)
			{
				std::cout << v << "  " << cube_indices[v] << std::endl;
			}

			cube_VAO = new VAO();

			cube_VBO = new VBO(cube_vertices);
			cube_IBO = new IBO(cube_indices);
			// Linking Vertex Attributes
			cube_VAO->Bind();
			cube_VAO->LinkAttrib(*cube_VBO, 0, 3, GL_FLOAT, sizeof(DUVertex), (void*)0);
			cube_VAO->LinkAttrib(*cube_VBO, 1, 3, GL_FLOAT, sizeof(DUVertex), (void*)(3* sizeof(float)));
			cube_VAO->LinkAttrib(*cube_VBO, 2, 2, GL_FLOAT, sizeof(DUVertex), (void*)(3* sizeof(float)));
			
			cube_shader = new MyShader("res/Shaders/Cube.vert.glsl", "res/Shaders/Cube.frag.glsl");
			cube_VAO->Unbind();
			cube_VBO->Unbind();
			cube_IBO->Unbind();
			cube_shader->Unbind();

			model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

			lightBox_shader = new MyShader("res/Shaders/Cube.vert.glsl", "res/Shaders/Cube.frag.glsl");


			// note that we're translating the scene in the reverse direction of where we want to move
			view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, eyePos));
			proj = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
		}

		~Test03_Box3D() {}

		void OnUpdate(float deltaTime = 0.0f) override {}

		void OnRender() override
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (is_wireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			

			m_tex0->Bind(0);
			m_shader->Bind();
			m_shader->SetUniformMat4f("u_model", model);
			m_shader->SetUniformMat4f("u_view", view);
			m_shader->SetUniformMat4f("u_proj", proj);
			m_shader->SetUniform3f("u_color", glm::vec3(rectColor[0], rectColor[1], rectColor[2]));
			m_shader->SetUniform1i("u_texture0", 0);
			m_VAO->Bind();
			m_IBO->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)nullptr);
			m_VAO->Unbind();
			m_IBO->Unbind();
			m_shader->Unbind();

			cube_shader->Bind();
			cube_VAO->Bind();
			cube_VBO->Bind();
			cube_model = glm::translate(model, glm::vec3(cube_translated[0], cube_translated[1], cube_translated[2]));
			cube_shader->SetUniformMat4f("u_MVP", proj * view * cube_model);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)nullptr);
			cube_shader->Unbind();
			cube_VAO->Unbind();
			cube_VBO->Unbind();

			lightBox_shader->Bind();
			cube_VAO->Bind();
			cube_VBO->Bind();
			cube_model = glm::translate(model, glm::vec3(cube_translated[0], cube_translated[1], 2.0f));
			lightBox_shader->SetUniformMat4f("u_MVP", proj * view * cube_model);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)nullptr);
			lightBox_shader->Unbind();
			cube_VAO->Unbind();
			cube_VBO->Unbind();
	
			view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, eyePos));
		}


		void OnImGuiRender() override 
		{ 
			ImGui::Text("Test03-Box3D"); 
			ImGui::SliderFloat3("Rectangle Color", rectColor, 0.0f, 1.0f);
			ImGui::SliderFloat("Eye Position", &eyePos, -100.0f, 0.0f);
			ImGui::SliderFloat3("Cube Position", cube_translated, -5.0f, 5.0f);
			ImGui::Checkbox("WireFrame", &is_wireFrame);
		}
	};

}
