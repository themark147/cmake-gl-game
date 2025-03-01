// Camera
// Object == StaticMesh/SkeletalMesh
// inputs?

#pragma once

#include "../Camera.h"
#include "../Input/KeyController.h"
#include "../ObjectSpawner.h"

namespace GLGame {
	class Player {
	public:
		Player(glm::vec3 position);
		void processInput();
		void processMouseInput();
		void processMovementInput();

		Camera& getCamera() {
			return camera;
		}

		void setSpawner(ObjectSpawner* spawner) {
			this->spawner = spawner;
		}
	private:
		Camera& camera = Camera();
		KeyInput::KeyController& keyController = KeyInput::KeyController::get();
		GLGame::ObjectSpawner* spawner = nullptr;
	};
}