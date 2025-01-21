#pragma once

#include <vector>
#include <reactphysics3d/reactphysics3d.h>

#include "../Object/Object.h"
#include "../Shader.h"

using namespace reactphysics3d;

namespace GLGame {
	class Scene {
	public:
		Scene(Shader &shader);
		void render();

		PhysicsCommon& getPhysicsCommon() {
			return physicsCommon;
		}

		PhysicsWorld* getWorld() {
			return world;
		}
	private:
		PhysicsCommon physicsCommon;
		PhysicsWorld* world;

		std::vector<Object *> objects;
		Shader shader;
	};
}