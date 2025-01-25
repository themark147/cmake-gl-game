#pragma once

#include <GLFW/glfw3.h>

namespace GLGame {
	class Application {
	public:
		Application::Application();
		
		void Init();
		void Run();

		static GLGame::Application& get() {
			static Application instance;

			return instance;
		}

		GLFWwindow* getWindow() {
			return window;
		}
	private:
		GLFWwindow* window = nullptr;
	};
}