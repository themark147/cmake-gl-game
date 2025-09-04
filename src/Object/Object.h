#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <reactphysics3d/reactphysics3d.h>

#include "../Model/Model.h"
#include "Transform.h"

using namespace reactphysics3d;

namespace GLGame {
	class Object
	{
	public:
		Object(PhysicsWorld* world, glm::vec3 position, BodyType type = BodyType::STATIC, CollisionShape *shape = nullptr, Model mesh = Model());

		void create(PhysicsWorld* world, BodyType type, CollisionShape *shape);
		glm::vec3 getPosition();
		Transform getTransform();
		RigidBody* getRigidBody();
		BoxShape* getShape();
		Vector3 toVector3Position();
		glm::mat4 getRotationMatrix();
		void render(Shader& shader);

		void setTransformation(GLGame::Transformation transform);

	private:
		Model mesh;
		glm::vec3 position;
		Transform transform;
		GLGame::Transformation transformation;
		BoxShape* shape;
		Collider* collider;
		RigidBody* rigidBody;
	};
}