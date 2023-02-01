#pragma once

#include "Test.hpp"
#include "Utility.hpp"

#include <GL/glew.h>
#include <imgui/imgui.h>

namespace test
{

class Scene01_ClearAWindow : public Test
{
	float m_ClearColor[4] = { 50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f };

public:
	Scene01_ClearAWindow() {}
	~Scene01_ClearAWindow() {}

	void OnUpdate(float deltaTime = 0.0f) override {}
	void OnRender() override
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	void OnImGuiRender() override { ImGui::ColorEdit4("Clear Color", m_ClearColor); }
};

}
