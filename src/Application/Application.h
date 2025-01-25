#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Shader.h"

namespace GLGame {
	class Application {
	public:
		Application::Application();

		int Init();
		void Run();

		static GLGame::Application& get() {
			static Application instance;

			return instance;
		}

		GLFWwindow* getWindow() {
			return window;
		}

		int getWidth();
		int getHeight();
	private:
		GLFWwindow* window = nullptr;
	};
};