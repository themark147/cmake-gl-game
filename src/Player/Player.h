#pragma once

#include "../Camera.h"
#include "../Input/KeyController.h"
#include "../ObjectSpawner.h"
#include "../Model/Model.h"

namespace GLGame {
	class Player {
	public:
		Player(glm::vec3 position, GLGame::Model mesh = Model());
		void processInput();
		void processMouseInput();
		void processMovementInput();

		Camera& getCamera() {
			return camera;
		}

		void setSpawner(ObjectSpawner* spawner) {
			this->spawner = spawner;
		}

		GLGame::Model getMesh() {
			return mesh;
		}
	private:
		Camera& camera = Camera();
		KeyInput::KeyController& keyController = KeyInput::KeyController::get();
		GLGame::ObjectSpawner* spawner = nullptr;
		GLGame::Model mesh;
	};
}