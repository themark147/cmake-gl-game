#include "Application.h"

#include <iostream>

int widthScreen = 1920;
int heightScreen = 1080;

GLGame::Scene* scene = nullptr;
GLGame::UI* ui = nullptr;

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
		ui = new GLGame::UI(window, scene);

		ui->init();

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
		ui->render();

		glfwSwapBuffers(window);
	}

	void Application::Shutdown() {
		ui->terminate();
		
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