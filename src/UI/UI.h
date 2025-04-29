#pragma once

#include "../Input/KeyController.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

namespace GLGame {
	class UI {
	public:
		UI(GLFWwindow* window, GLGame::Scene scene);
		void init();
		void render();
		void terminate();
	private:
		KeyInput::KeyController& keyController = KeyInput::KeyController::get();
		GLFWwindow* window = nullptr;
		GLGame::Scene scene;
	};
}