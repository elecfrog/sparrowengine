#include "Utility.hpp"
#include "Renderer.hpp"

#include "tests/Test.hpp"

// Test Scences Followed by https://learnopengl.com/
#include "tests/Scene01_ClearAWindow.hpp"
#include "tests/Scene02_DrawATriangle.hpp"
#include "tests/Scene03_TextureRectangle.hpp"
#include "tests/Scene04_RollingCubes.hpp"
#include "tests/Scene05_MyCamera.hpp."
#include "tests/Scene06_BasicLighting.hpp"

//#include "tests/Test-Texture2D.hpp"
//#include "tests/Test-Batching.hpp"
//#include "tests/Test-Batching-Textures.hpp"
//#include "tests/Test-Batching-Textures-dynamic.hpp"
//#include "tests/Test-FlashSquare2D.hpp"
//#include "tests/Test-Going3D.hpp"
//#include "tests/Test-Lighting.hpp"
//#include "tests/Test10-SpecularMaps.hpp"
//#include "tests/Test11-TypesofLight.hpp"
//
//#include "tests/Test01Ex-DrawARect.hpp"
//#include "tests/Test02-TextureBox2D.hpp"
//#include "tests/Test03-Box3D.hpp"
#include "tests/Test04-LightBox3D.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Init Width and Height of the window
static constexpr int window_width = 1920;
static constexpr int window_height = 1080;

// GLFW Window Object
GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// initialize GLFW & GLEW with Basic Information
int InitWindow() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwSetErrorCallback([](int error, const char *description) { std::cerr << "Error: " << description << std::endl; });

    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(window_width, window_height, "OpenGLRenderer", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(1);

    // Init GLEW (run-time OpenGL extensions loader)
    if (GLenum err = glewInit(); err != GLEW_OK) {
        std::cerr << "Error: glewInit(): " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
//    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwPollEvents();
//    glfwSetCursorPos(window, window_width / 2, window_height / 2);

    return 0;
}
void InitOpenGLFeatures()
{
    // Enable OpenGL features
    GLCall(glEnable(GL_DEBUG_OUTPUT));
    if (glIsEnabled(GL_DEBUG_OUTPUT) == GL_TRUE) // attempt to validate if gl-feature works
    GLCall(glDebugMessageCallback(GlDebugMessage_cb, nullptr));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
ImGuiIO& InitUI()
{
    // Init ImGui (graphical user interface for C++)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr);

    return io;
}
void PrintInfo()
{
    std::cout << "Info: GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "Info: GL   version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Info: GPU  vendor : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Info: Renderer    : " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Info: ImGui version: " << IMGUI_VERSION;

#ifdef IMGUI_HAS_VIEWPORT
    std::cout << " +viewport";
#endif

#ifdef IMGUI_HAS_DOCK
    std::cout << " +docking";
#endif
    std::cout << std::endl << std::endl;
}

int main(void)
{
    // Initialize and create a window.
    if (InitWindow()!= 0) return -1;
//    InitWindow();
    InitOpenGLFeatures();
    ImGuiIO& io = InitUI();
    PrintInfo();

	//disableCursor(window);

	{ // Vertex-/Index-Buffer scope
		Renderer renderer;

		test::Test *currentTest = nullptr;
		
		test::TestMenu *testMenu = new test::TestMenu(currentTest);
	
		currentTest = testMenu;

		testMenu->RegisterTest<test::Scene01_ClearAWindow>("Scene01: Clear A Window");
		testMenu->RegisterTest<test::Scene02_DrawATriangle>("Scene02: Draw A Triangle");
		testMenu->RegisterTest<test::Scene03_TextureRectangle>("Scene03: Draw A Texture Triangle");
        testMenu->RegisterTest<test::Scene04_RollingCubes>("Scene04: Rolling Cubes");
        testMenu->RegisterTest<test::Scene05_MyCamera>("Scene05: My Own Camera Class");
        testMenu->RegisterTest<test::Scene06_BasicLighting>("Scene06: Basic Lighting");


//		testMenu->RegisterTest<test::Test01Ex_DrawARect>("Test01-Ex Draw A Rect");
		//testMenu->RegisterTest<test::Test02_TextureBox2D>("Test02 Texture Box2D");
		//testMenu->RegisterTest<test::Test03_Box3D>("Test03 Box3D");
//		testMenu->RegisterTest<test::Test04_LightBox3D>("Test04 LightBox3D");
//		testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
//		testMenu->RegisterTest<test::Batching>("Batching");
//		testMenu->RegisterTest<test::BatchingTextures>("Batching Textures");
//		testMenu->RegisterTest<test::BatchingTexturesDynamic>("Batching Textures (dynamic)");
//		testMenu->RegisterTest<test::TestFlashSquare2D>("Test Flash Square 2D");
//		testMenu->RegisterTest<test::TestGoing3D>("Going 3D");
//		testMenu->RegisterTest<test::TestLighting>("Lighting");
//		testMenu->RegisterTest<test::Test10_SpecularMaps>("Test_10 SpecularMaps");
//		testMenu->RegisterTest<test::Test11_TypesofLight>("Test_11 Types of Light");
		
		bool show_demo_window = false;
		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			

			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate();
				currentTest->OnRender();
				ImGui::Begin("Test");
				
				if (currentTest != testMenu && ImGui::Button("Back"))
				{ // return to menu
					delete currentTest;
					currentTest = testMenu;
					// Dodge: GL_INVALID_VALUE error generated.
					//        <program> handle does not refer to an object generated by OpenGL.
					// on RenderPlatformWindowsDefault()->ImGui_ImplOpenGL3_RenderDrawData() which restores GL state after drawing: `glUseProgram(last_program);`
					GLCall(glUseProgram(0));
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			{ // Show a simple window that we create ourselves (use a Begin/End pair to created a named window)
				ImGui::Checkbox("Demo Window", &show_demo_window);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			if (show_demo_window) // Show the big demo window (documentation active samples)
				ImGui::ShowDemoWindow(&show_demo_window);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(window);
			}

			glfwSwapBuffers(window);
			glfwPollEvents();

		} // while (!glfwWindowShouldClose(window))

        // Check if the ESC key was pressed or the window was closed
        while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
               glfwWindowShouldClose(window) == 0);

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;

	} // Vertex-/Index-Buffer scope

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}