#include "Player.h"
#include "../Input/KeyController.h"

extern KeyInput::KeyController& controller;

namespace GLGame {
	Player::Player(glm::vec3 position) {
		camera = Camera(position);
		controller = KeyInput::KeyController::get();
	}

	void Player::processInput()
	{
		// TODO define & process inputs
	}
}