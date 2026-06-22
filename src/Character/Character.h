#pragma once

#include "../Object/Object.h"

namespace GLGame {
	class Character {
	public:
		Character(PhysicsWorld* world, PhysicsCommon& common);
		void render(Shader& shader);
	private:
		GLGame::Object* mesh;
	};
}