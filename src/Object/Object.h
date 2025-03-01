#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <reactphysics3d/reactphysics3d.h>

#include "../Model/Model.h"

using namespace reactphysics3d;

namespace GLGame {
	class Object
	{
	public:
		Object(PhysicsCommon& common, PhysicsWorld* world, glm::vec3 position, BodyType type = BodyType::STATIC, Model mesh = Model());

		void create(PhysicsCommon& common, PhysicsWorld* world, BodyType type, Vector3 size);
		glm::vec3 getPosition();
		Transform getTransform();
		RigidBody* getRigidBody();
		BoxShape* getShape();
		Vector3 toVector3Position();
		glm::mat4 getRotationMatrix();
		void render(Shader& shader);

	private:
		Model mesh;
		glm::vec3 position;
		Transform transform;
		BoxShape* shape;
		Collider* collider;
		RigidBody* rigidBody;
	};
}