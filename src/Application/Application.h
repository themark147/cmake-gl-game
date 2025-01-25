#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../Shader.h"
#include "../Scene/Scene.h"

namespace GLGame {
	class Application {
	public:
		Application::Application();

		int Init();
		void Run();
		void Shutdown();

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
		GLGame::Scene* scene = nullptr;
		KeyInput::KeyController& controller = KeyInput::KeyController::get();
	};
};