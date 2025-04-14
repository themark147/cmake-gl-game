#include "Player.h"
#include "../Input/KeyController.h"
#include "../Application/Application.h"

#include "../Object/CustomRaycastCallback.h";

namespace GLGame {
	float step = 1.0f / 100.0f;
	double x, y;

	float lastX = 1920 / 2.0f;
	float lastY = 1080 / 2.0f;
	bool firstMouse = true;
	bool stopVelocity = true;
	glm::vec3 direction = glm::vec3(0.0001f);
	glm::vec3 velocity = glm::vec3(0.00001f);

	bool isJumping = false;
	double startJump;


	Player::Player(glm::vec3 position, GLGame::Model mesh, reactphysics3d::PhysicsWorld* world) : world(world), mesh(mesh) {
		camera = Camera(position);
	}
	
	void Player::processInput()
	{
		processMouseInput();

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_F)) {
			glm::vec3 spawnPosition = (camera.Front * glm::vec3(10)) + camera.Position;

			spawner->createBox(spawnPosition);
		}

		Vector3 startPoint(camera.Position.x, camera.Position.y, camera.Position.z);
		Vector3 endPoint = (Vector3(camera.Position.x, camera.Position.y, camera.Position.z)) + Vector3(0.0f, -1.65f, 0.0f);
		Ray ray(startPoint, endPoint);

		GLGame::CustomRaycastCallback customRaycastCallback;
		world->raycast(ray, &customRaycastCallback);

		// std::cout << "Touching ground: " << customRaycastCallback.isOnGround << std::endl;

		//if (customRaycastCallback.isOnGround) {
			direction = glm::vec3(0.0001f);
		//}
		//else {
			//direction = glm::vec3(0.0001f, direction.y, 0.0001f);
		//}
		
		
		if (customRaycastCallback.isOnGround && startJump + 0.150f < glfwGetTime()) {
			if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_W)) {
				stopVelocity = true;
				camera.updateDirection(direction, Camera_Movement::FORWARD);
			}

			if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_A)) {
				stopVelocity = true;
				camera.updateDirection(direction, Camera_Movement::LEFT);
			}

			if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_S)) {
				stopVelocity = true;
				camera.updateDirection(direction, Camera_Movement::BACKWARD);
			}

			if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_D)) {
				stopVelocity = true;
				camera.updateDirection(direction, Camera_Movement::RIGHT);
			}

			isJumping = false;
		}

		velocity = camera.getVelocity(direction);

		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_SPACE) && customRaycastCallback.isOnGround) {
			// camera.ProcessKeyboard(JUMP, step);
			std::cout << "JUMPED" << velocity.x << " " << velocity.z << std::endl;
			velocity.y = 5.0f;
			// camera.updateDirection(direction, Camera_Movement::JUMP);
			collider.getRigidBody()->setLinearVelocity(
				reactphysics3d::Vector3(velocity.x, velocity.y, velocity.z)
			);

			isJumping = true;
			startJump = glfwGetTime();
		}

		if (
			!keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_W)
			&& !keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_A)
			&& !keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_S)
			&& !keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_D)
			// && !keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_SPACE)
			)
		{
			if (stopVelocity == true && !isJumping) {
				collider.getRigidBody()->setLinearVelocity(
					reactphysics3d::Vector3(0, 0, 0)
				);

				stopVelocity = false;
			}
		}
		else {
			if (customRaycastCallback.isOnGround && isJumping == false) { // (int(glfwGetTime() * 1000.0f) % 30 == 0)
				std::cout << "X: " << velocity.x << "Z: " << velocity.z << " " << glfwGetTime() * 1000.0f << std::endl;
				collider.getRigidBody()->setLinearVelocity(
					reactphysics3d::Vector3(velocity.x, velocity.y, velocity.z)
				);
			}
		}

		reactphysics3d::Vector3 pos = collider.getRigidBody()->getTransform().getPosition();
		camera.Position = glm::vec3(pos.x, pos.y, pos.z);
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
			camera.ProcessMouseMovement(xoffset, yoffset, step);
	}

	void Player::processMovementInput()
	{
		// because weird things happens when its zero
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