#include "Player.h"
#include "../Input/KeyController.h"

#include <iostream>

namespace GLGame {
	float step = 1.0f / 100.0f;

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
	}
}