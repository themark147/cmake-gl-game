#include "Object.h"

using namespace reactphysics3d;

namespace GLGame {
	Object::Object(PhysicsCommon& common, PhysicsWorld* world, glm::vec3 position, BodyType type, glm::vec3 size, Model mesh)
		: position(position), mesh(mesh)
	{
		transform = Transform(Vector3(position.x, position.y, position.z), Quaternion::identity());

		create(common, world, type, Vector3(size.x, size.y, size.z));
	}

	void Object::create(PhysicsCommon& common, PhysicsWorld* world, BodyType type, Vector3 size)
	{
		shape = common.createBoxShape(size);
		rigidBody = world->createRigidBody(transform);
		collider = rigidBody->addCollider(shape, Transform::identity());
		collider->setIsSimulationCollider(true);
		rigidBody->setType(type);
		rigidBody->setIsDebugEnabled(true);
		// rigidBody->applyLocalForceAtLocalPosition(Vector3(100, 100, 100) * Vector3(0, 1.5f, 0), Vector3(0.15, 0.7, 1.5));
	}

	glm::vec3 Object::getPosition()
	{
		// Display the position of the body
		// std::cout << "Body Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;

		Vector3 localRigidBodyPosition = rigidBody->getTransform().getPosition();
		/*Transform newTransform = rigidBody->getTransform();
		rp3d::Transform interpolatedTransform = rp3d::Transform::interpolateTransforms(transform, newTransform, 1.0f);
		transform = newTransform;

		Vector3 localRigidBodyPosition = interpolatedTransform.getPosition();*/

		return glm::vec3(localRigidBodyPosition.x, localRigidBodyPosition.y, localRigidBodyPosition.z);
	}

	Vector3 Object::toVector3Position()
	{
		return Vector3(position.x, position.x, position.x);
	}

	RigidBody* Object::getRigidBody()
	{
		return rigidBody;
	}

	Transform Object::getTransform()
	{
		return rigidBody->getTransform();
	}

	BoxShape* Object::getShape()
	{
		return shape;
	}

	glm::mat4 Object::getRotationMatrix()
	{
		Matrix3x3 transformMatrix = getTransform().getOrientation().getMatrix();
		// Matrix3x3 transformMatrix = getTransform().getInverse().getOrientation().getMatrix();
		Vector3 row0 = transformMatrix.getColumn(0);
		Vector3 row1 = transformMatrix.getColumn(1);
		Vector3 row2 = transformMatrix.getColumn(2);

		return glm::mat4(
			glm::mat3(
				(float)row0.x, (float)row0.y, (float)row0.z,
				(float)row1.x, (float)row1.y, (float)row1.z,
				(float)row2.x, (float)row2.y, (float)row2.z
			)
		);
	}

	void Object::render(Shader& shader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, getPosition());
		model = glm::scale(model, mesh.scale);

		float angle = glm::radians(180.0f); // Convert degrees to radians
		glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f); // X-axis
		model = glm::rotate(model, angle, axis);

		// lightingShader.setMat4("model", model * (*it)->getRotationMatrix());
		shader.setMat4("model", model * getRotationMatrix());
		mesh.Draw(shader);
	}
}
