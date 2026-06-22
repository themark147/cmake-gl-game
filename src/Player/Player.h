#pragma once

#include "../Camera.h"
#include "../Input/KeyController.h"
#include "../ObjectSpawner.h"
#include "../Model/Model.h"
#include "../Model/Collider.h"

#include <reactphysics3d/reactphysics3d.h>

namespace GLGame {
	class Player {
	public:
		Player(glm::vec3 position, GLGame::Model mesh, reactphysics3d::PhysicsWorld* world, PhysicsCommon& common);
		void processInput();
		void processMouseInput();
		void processMovementInput();
		void processRayCast(RaycastCallback &callback);
		void resetLocation();
		void draw(Shader& shader, glm::mat4& projection, glm::mat4& view);

		Camera& getCamera() {
			return camera;
		}

		GLGame::Model getMesh() {
			return mesh;
		}

		void setCollider(GLGame::Collider collider) {
			this->collider = collider;

			/*reactphysics3d::Material& material = collider.getRigidBody()->getCollider(0)->getMaterial();
			material.setFrictionCoefficient(20.5f); // Friction coefficient
			material.setBounciness(0.0f); // Bounciness coefficient*/
		}

		GLGame::Collider getCollider() {
			return collider;
		}

		void setWorld(reactphysics3d::PhysicsWorld* world) {
			this->world = world;
		}

		void setSpawner(GLGame::ObjectSpawner* spawner) {
			this->spawner = spawner;
		}
	private:
		glm::mat4 applyTransform(Shader& shader);
		
		Camera& camera = Camera();
		KeyInput::KeyController& keyController = KeyInput::KeyController::get();
		GLGame::ObjectSpawner* spawner = nullptr;
		reactphysics3d::PhysicsWorld* world;
		reactphysics3d::PhysicsCommon& common;
		GLGame::Model mesh;
		GLGame::Collider collider;
	};
}