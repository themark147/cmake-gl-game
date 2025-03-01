#include "Application.h"

#include <iostream>

int widthScreen = 1920;
int heightScreen = 1080;
ImGuiIO* io = nullptr;

GLGame::Scene* scene = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	widthScreen = width;
	heightScreen = height;

	glViewport(0, 0, width, height);
}

namespace GLGame {
	Application::Application() {
		window = glfwCreateWindow(widthScreen, heightScreen, "Greatest OpenGL application", NULL, NULL);
	}

	int Application::Init()
	{
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();

			return -1;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;

			return -1;
		}

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glEnable(GL_DEPTH_TEST);
		glfwSwapInterval(0); // vsync

		stbi_set_flip_vertically_on_load(true); // Because of textures

		scene = new GLGame::Scene();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO(); (void)io;
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui::StyleColorsLight();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		return 0;
	}

	void Application::Run()
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		
		if (controller.isKeyPressed(KeyInput::KeyDefinition::KEY_TAB)) {
			scene->getWorld()->setIsDebugRenderingEnabled(true);
		}

		controller.processKeys(window);

		glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->render();

		if (controller.isKeyPressed(KeyInput::KeyDefinition::KEY_T)) {
		    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Main window");
			// ImGui::SliderFloat("float", &XLight, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			// ImGui::SliderFloat3("floatt", &light.x, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
			ImGui::End();

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		} else {
		    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	    }

		glfwSwapBuffers(window);
	}

	void Application::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		
		glfwTerminate();
	}

	int Application::getWidth()
	{
		return widthScreen;
	}

	int Application::getHeight()
	{
		return heightScreen;
	}
};