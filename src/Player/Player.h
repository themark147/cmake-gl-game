// Camera
// Object == StaticMesh/SkeletalMesh
// inputs?

#pragma once

#include "../Camera.h"
#include "../Input/KeyController.h"

namespace GLGame {
	class Player {
	public:
		Player::Player(glm::vec3 position);
		void processInput();

		Camera& getCamera() {
			return camera;
		}
	private:
		Camera camera;
		KeyInput::KeyController& keyController = KeyInput::KeyController::get();
	};
}