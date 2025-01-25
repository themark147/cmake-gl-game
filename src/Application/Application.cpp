#include "Application.h"

namespace GLGame {
	Application::Application() {
		Init();
	}

	void Application::Init()
	{
		window = glfwCreateWindow(1920, 1080, "Greatest OpenGL application", NULL, NULL);
	}

	void Application::Run()
	{
	}
}