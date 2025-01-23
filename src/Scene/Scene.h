#pragma once

#include <vector>
#include <reactphysics3d/reactphysics3d.h>

#include "../Object/Object.h"
#include "../Player/Player.h"

#include "../Shader.h"
#include "../Camera.h"

using namespace reactphysics3d;

namespace GLGame {
	class Scene {
	public:
		Scene(Shader& shader);
		void render();

		PhysicsCommon& getPhysicsCommon() {
			return physicsCommon;
		}

		PhysicsWorld* getWorld() {
			return world;
		}
		
		GLGame::Player& getPlayer() {
			return player;
		}
	private:
		PhysicsCommon physicsCommon;
		PhysicsWorld* world;

		std::vector<Object *> objects;

		GLGame::Player player = GLGame::Player(glm::vec3(0.0f, 0.0f, 15.0f));
		Camera& camera = Camera();

		Shader shader;
	};
}