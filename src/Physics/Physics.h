#pragma once

#include "../Common/OpenGL.h"

#include <reactphysics3d/reactphysics3d.h>
#include <chrono>

using namespace reactphysics3d;

namespace GLGame {
	class Physics {
	public:
		Physics();
		bool tick();

		PhysicsCommon& getPhysicsCommon() {
			return physicsCommon;
		}

		PhysicsWorld* getWorld() {
			return world;
		}
	private:
		void init();

		PhysicsCommon physicsCommon;
		PhysicsWorld* world;
	};
}