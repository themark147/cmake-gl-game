#include "Scene.h";

#include <reactphysics3d/reactphysics3d.h>

#include "../Object/Object.h"
#include "../Shader.h"

#include <vector>

using namespace reactphysics3d;

namespace GLGame {
	Scene::Scene(Shader &shader) : shader(shader) {
		// Init physics
		world = physicsCommon.createPhysicsWorld();

		// Init objects
		objects.push_back(new GLGame::Object(glm::vec3(0.0f, -2.0f, 0.0f)));

		// TODO: should be part of Object construct
		for (std::vector<GLGame::Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			(*it)->create(physicsCommon, world, BodyType::STATIC, Vector3(10, 1, 10)); // last param only convex SIZE
		}
	}
	
	void Scene::render()
	{
		for (std::vector<GLGame::Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, (*it)->getPosition());
			
			// lightingShader.setMat4("model", model * (*it)->getRotationMatrix());
			shader.setMat4("model", model);
			(*it)->render();
		}
	}
}