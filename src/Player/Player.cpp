#include "Player.h"
#include "../Input/KeyController.h"

namespace GLGame {
	Player::Player(glm::vec3 position) {
		camera = Camera(position);
	}
	
	void Player::processInput()
	{
		if (keyController.isKeyPressed(KeyInput::KeyDefinition::KEY_A)) {
			camera.ProcessKeyboard(LEFT, 1.0f / 100.0f); // TODO step same as step from physics ?
		}
	}
}