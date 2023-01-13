#pragma once

#include "Test.hpp"
#include "Utility.hpp"

#include "MyClasses.hpp"

#include <GL/glew.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace test
{

	class Test01Ex_DrawARect : public Test
	{
		// create a OpenGL Objects: VAO, VBO, ShaderProgram
		VBO* m_VBO;
		IBO* m_IBO;
		VAO* m_VAO;
		//unsigned int IBO;
		unsigned int shader_program;
		MyShader* myShaer;


		float rectColor[3] = { 1.0f, 0.0f, 1.0f	};
		bool is_wireFrame = false;

	public:
		Test01Ex_DrawARect()
		{
			float vertex_data[] = {
 				 0.5f,  0.5f, 0.0f,  // top right
  				 0.5f, -0.5f, 0.0f,  // bottom right
				-0.5f, -0.5f, 0.0f,  // bottom left
				-0.5f,  0.5f, 0.0f   // top left 
			};

			unsigned int indices[] = {  // note that we start from 0!
				0, 1, 3,  // first Triangle
				1, 2, 3   // second Triangle
			};


			m_VAO = new VAO();
			m_VBO = new VBO(vertex_data, sizeof(vertex_data));
			m_IBO = new IBO(indices, sizeof(indices));

			// Linking Vertex Attributes
			m_VAO->LinkAttrib(*m_VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

			myShaer = new MyShader("res/Shaders/vertex.glsl", "res/Shaders/frag.glsl");
		}

		~Test01Ex_DrawARect() {}

		void OnUpdate(float deltaTime = 0.0f) override {}

		void OnRender() override
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			myShaer->Bind();
			myShaer->SetUniform3f("u_color", glm::vec3(rectColor[0], rectColor[1], rectColor[2]));
			//glUseProgram(shader_program);
			m_VAO->Bind();
			if (is_wireFrame)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)nullptr);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		void OnImGuiRender() override 
		{ 
			ImGui::Text("Test01-EX Draw A Rectangle"); 
			ImGui::SliderFloat3("Rectangle Color", rectColor, 0.0f, 1.0f);
			ImGui::Checkbox("WireFrame", &is_wireFrame);
		}
	};

}
