#include "Application.h"

#include <iostream>

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
	}

	void Application::Run()
	{
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