#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace GLGame {
	class Collider {
	public:
		// Default constructor
		Collider() : rigidBody(nullptr) {}

		Collider(
			reactphysics3d::PhysicsCommon& common,
			reactphysics3d::PhysicsWorld* world,
			glm::vec3 transform
		) {
			reactphysics3d::CapsuleShape* shape = common.createCapsuleShape(1.0f, 1.0f);
			rigidBody = world->createRigidBody(
				reactphysics3d::Transform(Vector3(transform.x, transform.y, transform.z), reactphysics3d::Quaternion::identity())
			);
			reactphysics3d::Collider* collider = rigidBody->addCollider(shape, reactphysics3d::Transform::identity());
			collider->setIsSimulationCollider(true);
			rigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
			rigidBody->setIsDebugEnabled(true);
		}

		reactphysics3d::RigidBody* getRigidBody() {
			return rigidBody;
		}
	private:
		reactphysics3d::RigidBody* rigidBody;
	};
}