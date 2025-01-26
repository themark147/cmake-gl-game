#pragma once

#include "Object/Object.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace GLGame {
	class ObjectSpawner {
	public:
        ObjectSpawner(PhysicsCommon& common, PhysicsWorld* world, std::vector<Object>& objects) : common(common), world(world), objects(objects) {

        }

        void createBox(glm::vec3 spawnPosition)
        {
            GLGame::Object object(spawnPosition);
            
            object.create(common, world, BodyType::DYNAMIC, Vector3(1, 1, 1));
            objects.push_back(object);
            // TODO forward vector
            object.getRigidBody()->applyLocalForceAtLocalPosition(Vector3(100, 100, 100) * Vector3(0, 1.5f, 0), Vector3(0.15, 0.7, 1.5));
        }
    private:
        PhysicsCommon& common;
        PhysicsWorld* world;
        std::vector<Object> objects;
	};
}