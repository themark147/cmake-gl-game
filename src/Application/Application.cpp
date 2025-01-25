#include "Application.h"

#include <iostream>

#include "../Scene/Scene.h"

extern std::string vertexShaderDebug;
extern std::string fragmentShaderDebug;

int widthScreen = 1920;
int heightScreen = 1080;

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

		Shader debugShader(vertexShaderDebug, fragmentShaderDebug);
		GLGame::Scene scene = GLGame::Scene(debugShader);

		return 0;
	}

	void Application::Run()
	{
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//if (controller.isKeyPressed(KeyInput::KeyDefinition::KEY_TAB)) {
			//world->setIsDebugRenderingEnabled(true);
		//}
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