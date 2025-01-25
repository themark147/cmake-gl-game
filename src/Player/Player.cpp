#include "Player.h"
#include "../Input/KeyController.h"
#include "../Application/Application.h"

#include <iostream>

namespace GLGame {
	float step = 1.0f / 100.0f;
	double x, y;

	float lastX = 1920 / 2.0f;
	float lastY = 1080 / 2.0f;
	bool firstMouse = true;

	Player::Player(glm::vec3 position) {
		camera = Camera(position);
	}
	
	void Player::processInput()
	{
		// TODO step same as step from physics ?
		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_W)) {
			std::cout << "Forward\n";
			camera.ProcessKeyboard(FORWARD, step);
		}

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_A)) {
			camera.ProcessKeyboard(LEFT, step); 
		}

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_S)) {
			camera.ProcessKeyboard(BACKWARD, step);
		}

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_D)) {
			camera.ProcessKeyboard(RIGHT, step);
		}

		processMouseInput();
	}

	void Player::processMouseInput()
	{
		GLFWwindow* window = GLGame::Application::get().getWindow();
		glfwGetCursorPos(window, &x, &y);

		float xpos = static_cast<float>(x);
		float ypos = static_cast<float>(y);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;
		
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			camera.ProcessMouseMovement(xoffset, yoffset);
	}
}