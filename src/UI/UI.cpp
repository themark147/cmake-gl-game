#include "UI.h"
#include "../Scene/Scene.h"

ImGuiIO* io = nullptr;

namespace GLGame {
	UI::UI(GLFWwindow* window, GLGame::Scene* scene) : window(window), scene(scene)
	{
	}

	void UI::init()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = &ImGui::GetIO(); (void)io;
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui::StyleColorsLight();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void UI::render()
	{
		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_T)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Main window");

			if (ImGui::Button("Reset player location")) {
				scene->getPlayer().resetLocation();
			}
			// ImGui::SliderFloat("float", &XLight, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			// ImGui::SliderFloat3("floatt", &light.x, -50.0f, 50.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
			ImGui::End();

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	void UI::terminate()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}