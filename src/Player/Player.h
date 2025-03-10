#pragma once

#include "../Camera.h"
#include "../Input/KeyController.h"
#include "../ObjectSpawner.h"
#include "../Model/Model.h"
#include "../Model/Collider.h"

namespace GLGame {
	class Player {
	public:
		Player(glm::vec3 position, GLGame::Model mesh = Model());
		void processInput();
		void processMouseInput();
		void processMovementInput();
		glm::mat4& applyTransform(Shader& shader, glm::vec3 offset = glm::vec3(0.0f));

		Camera& getCamera() {
			return camera;
		}

		void setSpawner(ObjectSpawner* spawner) {
			this->spawner = spawner;
		}

		GLGame::Model getMesh() {
			return mesh;
		}

		void setCollider(GLGame::Collider collider) {
			this->collider = collider;
		}

		GLGame::Collider getCollider() {
			return collider;
		}
	private:
		glm::vec3 applyMeshOffset(Camera& camera, glm::vec3& offset);
		
		Camera& camera = Camera();
		KeyInput::KeyController& keyController = KeyInput::KeyController::get();
		GLGame::ObjectSpawner* spawner = nullptr;
		GLGame::Model mesh;
		GLGame::Collider collider;
	};
}