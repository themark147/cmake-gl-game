#pragma once

#include "Object/Object.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace GLGame {
	class ObjectSpawner {
	public:
        ObjectSpawner(PhysicsWorld* world, PhysicsCommon &common, std::vector<Object> &objects) : world(world), common(common), objects(objects) {
            // this->objects = objects;
        }

        void createBox(glm::vec3 spawnPosition, glm::vec3 direction)
        {
            // GLGame::Object* object = nullptr;
            
            GLGame::Object object(
                world,
                spawnPosition,
                BodyType::DYNAMIC,
                common.createBoxShape(Vector3(1, 1, 1)),
                // common.createCapsuleShape(.5f, 1.22f)
                Model(std::string("birthday_love_letter.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(0.01f))
            );            

            // TODO forward vector
            object.getRigidBody()->applyLocalForceAtLocalPosition(Vector3(1000, 1000, 1000) * Vector3(direction.x, direction.y, direction.z), Vector3(0.15, 0.7, 1.5));

            objects.push_back(
                object
            );

            printf("Pocet objects: {}", objects.size());
        }
    private:
        PhysicsWorld* world;
        PhysicsCommon& common;
        std::vector<Object> &objects;
	};
}