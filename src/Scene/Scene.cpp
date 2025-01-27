#include "Scene.h";

#include <reactphysics3d/reactphysics3d.h>

#include "../Application/Application.h"
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

std::chrono::duration<double> timeStep = std::chrono::duration<double>(1.0f / 100.0f);

namespace GLGame {
	Scene::Scene(Shader& shader) : shader(shader) {
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
		objects.push_back(GLGame::Object(glm::vec3(0.0f, -2.0f, 0.0f)));

		// TODO: should be part of Object construct

		for (GLGame::Object& obj : objects) {
			obj.create(physicsCommon, world, BodyType::STATIC, Vector3(10, 1, 10)); // last param only convex SIZE
		}
		
		models.push_back(Model("../../../resources/zombie_char_7_4.glb", glm::vec3(0.0f, 0.0f, 0.0f)));
		// models.push_back(Model("../../../resources/sword.glb", glm::vec3(0.0f, 0.0f, 0.0f)));
	}
	
	void Scene::render()
	{
		camera = player.getCamera();
		player.processInput();

		shader.use();

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

		int vertexPositionLoc = shader.getAttribLocation("aPos");
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
		shader.setMat4("projection", projection);
		
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);

		for (GLGame::Object obj : objects) {
			// std::cout << "Pocet objects: " << objects.size() << "\n";
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, obj.getPosition());

			// lightingShader.setMat4("model", model * (*it)->getRotationMatrix());
			shader.setMat4("model", model);
			// obj.render();
		}

		mainShader.use();

		mainShader.setMat4("projection", projection);
		mainShader.setMat4("view", view);

		mainShader.setVec3("viewPos", camera.Position);

		mainShader.setVec3("lightPos", 10.0f, 7.0f, 20.0f);
		mainShader.setVec3("lightColor", 0.5f, 0.5f, 0.5f);

		for (Model meshModel : models) {
			// std::cout << "Pocet: " << models.size();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -2.0f, 13.0f));

			// lightingShader.setMat4("model", model * (*it)->getRotationMatrix());
			mainShader.setMat4("model", model);
			meshModel.Draw(mainShader);
		}
	}
}