#include "Scene.h"

#include "../Application/Application.h"
#include "../Model/Model.h"
#include "../Object/Object.h"
#include "../Shader.h"

#include <vector>
#include <iostream>

namespace GLGame {
	Scene::Scene() {
		reactphysics3d::PhysicsWorld* world = physics.getWorld();
		reactphysics3d::PhysicsCommon& common = physics.getPhysicsCommon();

		// Init player
		player.setSpawner(new GLGame::ObjectSpawner(world, objects));
		player.setWorld(world);
		player.setCollider(GLGame::Collider(common, world, glm::vec3(player.getCamera().Position)));

		// Select the contact points and contact normals to be displayed
		world->getDebugRenderer().setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);	

		// Init objects
		GLGame::Object zombie = GLGame::Object(
			world,
			glm::vec3(0.0f, 4.0f, 10.0f),
			BodyType::DYNAMIC,
			common.createCapsuleShape(.5f, 1.22f),
			Model(std::string("zombie_w_anim.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(.0002f))
		);
		zombie.setTransformation(GLGame::Transformation(glm::radians(180.0f)));
		objects.push_back(zombie);
		
		objects.push_back(GLGame::Object(
			world,
			glm::vec3(0.0f, -2.0f, 10.0f),
			BodyType::STATIC,
			common.createBoxShape(Vector3(10.0f, 0.20f, 10.0f)),
			Model(std::string("first_person_arms_pivot.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(2.0f))
		));

		objects.push_back(GLGame::Object(
			world,
			glm::vec3(-21.5f, -1.0f, 10.0f),
			BodyType::STATIC,
			common.createBoxShape(Vector3(10.0f, 0.20f, 10.0f)),
			Model(std::string("first_person_arms_pivot.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(2.0f))
		));

		objects.push_back(GLGame::Object(
			world,
			glm::vec3(21.5f, -3.0f, 10.0f),
			BodyType::STATIC,
			common.createBoxShape(Vector3(10.0f, 0.20f, 10.0f)),
			Model(std::string("first_person_arms_pivot.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(2.0f))
		));

		objects.push_back(GLGame::Object(
			world,
			glm::vec3(0.0f, -3.5f, 31.0f),
			BodyType::STATIC,
			common.createBoxShape(Vector3(10.0f, 0.20f, 10.0f)),
			Model(std::string("first_person_arms_pivot.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(2.0f))
		));

		objects.push_back(GLGame::Object(
			world,
			glm::vec3(25.0f, -1.0f, 15.0f),
			BodyType::STATIC,
			common.createBoxShape(Vector3(10.0f, 0.20f, 10.0f)),
			Model(std::string("plane.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(2.0f))
		));

		mainShader.use();
		mainShader.setVec3("lightColor", 0.5f, 0.5f, 0.5f);
		mainShader.setVec3("lightDir", -0.5f, -0.5f, -0.5f);

		// LAST texture of shader
		mainShader.setInt("shadowMap", 15);

		// Init shadow map
		shadowMap.init();
	}
	
	void Scene::render()
	{
		camera = player.getCamera();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)GLGame::Application::get().getWidth() / (float)GLGame::Application::get().getHeight(), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		player.processInput();

		if (physics.tick()) {
			player.processMovementInput();
		}

		// 1. Render to shadow map
		shadowMap.render(simpleDepthShader, camera.Position);
		
		player.draw(simpleDepthShader, projection, view);
		for (GLGame::Object obj : objects) {
			obj.render(simpleDepthShader);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);		

		// reset viewport
		glViewport(0, 0, GLGame::Application::get().getWidth(), GLGame::Application::get().getHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. Debug
		debugShader.use();

		debugRender.Show(physics.getWorld(), debugShader, camera, projection, view);

		// 3. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		mainShader.use();

		// mainShader.setMat4("projection", projection);
		// mainShader.setMat4("view", view);
		// set light uniforms
		glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
		mainShader.setVec3("lightPos", -2.0f, 4.0f, -1.0f);
		mainShader.setMat4("lightSpaceMatrix", shadowMap.getLightSpaceMatrix());

		player.draw(mainShader, projection, view);

		for (GLGame::Object obj : objects) {
			obj.render(mainShader);
		}
	}
}