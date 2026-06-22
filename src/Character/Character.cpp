#include "Character.h"

namespace GLGame {
	Character::Character(PhysicsWorld* world, PhysicsCommon& common) {
		mesh = new GLGame::Object(
			world,
			glm::vec3(0.0f, -1.5f, 8.0f),
			BodyType::STATIC,
			common.createCapsuleShape(.5f, 1.22f),
			Model(std::string("SM_Workbench.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(1.0f))
		);

		mesh->setTransformation(GLGame::Transformation(glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)));
	}

	void Character::render(Shader& shader)
	{
		mesh->render(shader);
	}
}