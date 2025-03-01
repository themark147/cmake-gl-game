#include "Scene.h";

#include <reactphysics3d/reactphysics3d.h>

#include "../Application/Application.h"
#include "../Model/Model.h"
#include "../Object/Object.h"
#include "../Shader.h"

#include <vector>
#include <iostream>

using namespace reactphysics3d;

using chrono_clock = std::chrono::high_resolution_clock;

std::chrono::time_point<chrono_clock> mStartTime;
std::chrono::time_point<std::chrono::high_resolution_clock> mLastUpdateTime;

/// Used to fix the time step and avoid strange time effects
std::chrono::duration<double> mAccumulator;
std::chrono::duration<double> deltaTime;

std::chrono::duration<double> timeStep = std::chrono::duration<double>(1.0f / 60.0f);

namespace GLGame {
	Scene::Scene() {
		// TODO part of Physics.cpp -> tick() -> step()
		mStartTime = std::chrono::high_resolution_clock::now();
		mLastUpdateTime = mStartTime;
		mAccumulator = std::chrono::duration<double>::zero();

		// Init physics
		world = physicsCommon.createPhysicsWorld();
		player.setSpawner(new GLGame::ObjectSpawner(physicsCommon, world, objects));

		initDebug();

		// Select the contact points and contact normals to be displayed
		world->getDebugRenderer().setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);

		// Init objects

		objects.push_back(GLGame::Object(
			physicsCommon,
			world,
			glm::vec3(0.0f, -2.0f, 10.0f),
			BodyType::STATIC,
			glm::vec3(10.0f, 0.2f, 10.0f),
			Model("../../../resources/FirstPersonMap.glb", glm::vec3(2.0f))
		));

		objects.push_back(GLGame::Object(
			physicsCommon,
			world,
			glm::vec3(0.0f, 1.0f, 10.0f),
			BodyType::DYNAMIC,
			glm::vec3(1.0f),
			Model("../../../resources/zombie_w_anim.glb", glm::vec3(.0002f))
		));

		mainShader.use();
		mainShader.setVec3("lightPos", 10.0f, 7.0f, 20.0f);
		mainShader.setVec3("lightColor", 0.5f, 0.5f, 0.5f);
		mainShader.setVec3("lightDir", -0.5f, -0.5f, -0.5f);

		// models.push_back(Model("../../../resources/zombie_another_anim.glb", glm::vec3(0.0f, 0.0f, 0.0f)));
		// models.push_back(Model("../../../resources/turret.glb", glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	
	void Scene::render()
	{
		camera = player.getCamera();
		player.processInput();

		debugShader.use();

		// ----- Triangles ---- //
		const uint nbTriangles = world->getDebugRenderer().getNbTriangles();

		if (nbTriangles > 0)
		{
			// Vertices
			mDebugVBOTrianglesVertices.bind();
			GLsizei sizeVertices = static_cast<GLsizei>(nbTriangles * sizeof(rp3d::DebugRenderer::DebugTriangle));
			mDebugVBOTrianglesVertices.copyDataIntoVBO(sizeVertices, world->getDebugRenderer().getTrianglesArray(), GL_STREAM_DRAW);
			mDebugVBOTrianglesVertices.unbind();
		}

		int vertexPositionLoc = debugShader.getAttribLocation("aPos");
		// int vertexColorLoc = debugShader.getAttribLocation("vertexColor");

		// Triangles
		if (nbTriangles > 0) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			drawDebug(world->getDebugRenderer(), vertexPositionLoc, 2);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = currentTime - mLastUpdateTime;

		// Update the current display time
		mLastUpdateTime = currentTime;
		mAccumulator += deltaTime;		

		while (mAccumulator >= timeStep) {
			// mainShader.setVec3("light.position", light.x, light.y, light.z); // ImGui
			world->update(timeStep.count());
			player.processMovementInput();

			mAccumulator -= timeStep;
		}

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)GLGame::Application::get().getWidth() / (float)GLGame::Application::get().getHeight(), 0.1f, 100.0f);
		debugShader.setMat4("projection", projection);
		
		glm::mat4 view = camera.GetViewMatrix();
		debugShader.setMat4("view", view);

		mainShader.use();

		mainShader.setMat4("projection", projection);
		mainShader.setMat4("view", view);
		mainShader.setVec3("viewPos", camera.Position);

		for (GLGame::Object obj : objects) {
			obj.render(mainShader);
		}
	}
}