#include "Player.h"
#include "../Input/KeyController.h"
#include "../Application/Application.h"

namespace GLGame {
	float step = 1.0f / 100.0f;
	double x, y;

	float lastX = 1920 / 2.0f;
	float lastY = 1080 / 2.0f;
	bool firstMouse = true;
	bool stopVelocity = true;

	Player::Player(glm::vec3 position, GLGame::Model mesh) : mesh(mesh) {
		camera = Camera(position);
	}
	
	void Player::processInput()
	{
		processMouseInput();

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_F)) {
			glm::vec3 spawnPosition = (camera.Front * glm::vec3(10)) + camera.Position;

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
		glm::vec3 direction = glm::vec3(0.0f);
		float speed = 0;

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_W)) {
			speed = 5;
			direction.z = 1.0f;
			stopVelocity = true;
			camera.ProcessKeyboard(FORWARD, step);
		}

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_A)) {
			speed = 5;
			direction.x = -1.0f;
			stopVelocity = true;
			camera.ProcessKeyboard(LEFT, step);
		}

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_S)) {
			speed = 5;
			direction.z = -1.0f;
			stopVelocity = true;
			camera.ProcessKeyboard(BACKWARD, step);
		}

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_D)) {
			speed = 5;
			direction.x = 1.0f;
			stopVelocity = true;
			camera.ProcessKeyboard(RIGHT, step);
		}

		if (
			!keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_W)
			&& !keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_A)
			&& !keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_S)
			&& !keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_D)
		)
		{
			if (stopVelocity == true) {
				collider.getRigidBody()->setLinearVelocity(
					reactphysics3d::Vector3(0, 0, 0)
				);

				stopVelocity = false;
			}
		}
		else {
			// Move direction and speed right into camera
			glm::vec3 velocity = camera.getVelocity(step, direction) * (float) speed;
			collider.getRigidBody()->setLinearVelocity(
				reactphysics3d::Vector3(velocity.x, velocity.y, velocity.z)
			);
		}

		reactphysics3d::Vector3 pos = collider.getRigidBody()->getTransform().getPosition();
		camera.Position = glm::vec3(pos.x, pos.y, pos.z);

		speed = 0;
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