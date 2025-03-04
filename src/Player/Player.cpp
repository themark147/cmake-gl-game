#include "Player.h"
#include "../Input/KeyController.h"
#include "../Application/Application.h"

namespace GLGame {
	float step = 1.0f / 100.0f;
	double x, y;

	float lastX = 1920 / 2.0f;
	float lastY = 1080 / 2.0f;
	bool firstMouse = true;

	Player::Player(glm::vec3 position, GLGame::Model mesh) : mesh(mesh) {
		camera = Camera(position);
	}
	
	void Player::processInput()
	{
		processMouseInput();

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_F)) {
			glm::vec3 spawnPosition = (camera.Front * glm::vec3(15)) + camera.Position;

			spawner->createBox(spawnPosition);
		}
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

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_SPACE)) {
			camera.ProcessKeyboard(JUMP, step);
		}
		
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			camera.ProcessMouseMovement(xoffset, yoffset, step);
	}

	void Player::processMovementInput()
	{
		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_W)) {
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
	}

	glm::mat4& Player::applyTransform(Shader& shader, glm::vec3 offset)
	{
		glm::mat4 playerMesh = glm::mat4(1.0f);
		glm::vec3 modelPosition = applyMeshOffset(camera, offset);

		playerMesh = glm::translate(playerMesh, modelPosition);
		playerMesh = glm::scale(playerMesh, glm::vec3(.02f));

		playerMesh = glm::rotate(playerMesh, -glm::radians(camera.Yaw) + glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		playerMesh = glm::rotate(playerMesh, -glm::radians(camera.Pitch), glm::vec3(1.0f, 0.0f, 0.0f));

		shader.setMat4("model", playerMesh);
		mesh.Draw(shader);

		return playerMesh;
	}

	glm::vec3 Player::applyMeshOffset(Camera& camera, glm::vec3& offset)
	{
		glm::vec3 modelPosition = camera.Position;

		modelPosition += camera.Front * offset.z;  // Move along the camera's forward direction
		modelPosition += camera.Right * offset.x;  // Move along the camera's right direction
		modelPosition += camera.Up * offset.y; // -||- up direction

		return modelPosition;
	}
}