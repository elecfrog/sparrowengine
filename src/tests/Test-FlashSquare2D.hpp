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

	class TestFlashSquare2D : public Test
	{
		std::unique_ptr<VertexArray> vao;
		std::unique_ptr<IndexBuffer> ibo;
		std::unique_ptr<VertexBuffer> vbo;
		std::unique_ptr<Shader> m_shader;

		Renderer renderer;

		float r = 0.f;
		float increment = 0.05f;


	public:
		TestFlashSquare2D() 
		{
			float positions[8] = {
				// 2 bits
				-0.5f, -0.5f,
				0.5f, -0.5f,
				0.5f, 0.5f,
				-0.5f, 0.5f
			};

			unsigned int indices[6] = {
				0,1,2,
				2,3,0,
			};


			vao = std::make_unique<VertexArray>();

			vbo = std::make_unique<VertexBuffer>(positions, 4 * 2 * sizeof(float));
			VertexBufferLayout layout;
			layout.Push<float>(2);
			vao->AddBuffer(*vbo, layout);

			ibo = std::make_unique<IndexBuffer>(indices, 6);

			m_shader = std::make_unique<Shader>("res/shaders/Triangle.shader");
			m_shader->Bind();

			m_shader->SetUniform4f("u_Color", 1.0f, 0.0f, 0.f, 1.f);
			

			vbo->Unbind();
			ibo->Unbind();
			vao->Unbind();
			m_shader->Unbind();
		}

		~TestFlashSquare2D() {}

		void OnUpdate(float deltaTime = 0.0f) override {}
		
		void OnRender() override
		{
			renderer.Clear();

			m_shader->Bind();
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;
			m_shader->SetUniform4f("u_Color", r, 0.0f, 0.f, 1.f);

			renderer.Draw(*vao, *ibo, *m_shader);
		}
		void OnImGuiRender() override { ImGui::Text("Test Flash Square 2D"); }
	};

}
