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

unsigned int depthMapFBO;
// create depth texture
unsigned int depthMap;

const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;

const glm::vec3 G_LIGHT_SOURCE_ORIGINAL_POS(-2.0f, 4.0f, -1.0f);
const glm::vec3 G_LIGHT_DIRECTION = glm::normalize(glm::vec3(0.0f) - G_LIGHT_SOURCE_ORIGINAL_POS);
const float G_SHADOW_ORTHO_WIDTH = 20.0f;
const float G_SHADOW_ORTHO_HEIGHT = 20.0f;
const float G_SHADOW_BOX_DEPTH = 15.0f; // Desired depth of the shadow box
const float G_SHADOW_ORTHO_NEAR_OFFSET = 0.1f; // Near plane for ortho, relative to light's new eye
const float G_SHADOW_ORTHO_FAR_OFFSET = G_SHADOW_ORTHO_NEAR_OFFSET + G_SHADOW_BOX_DEPTH;

namespace GLGame {
	Scene::Scene() {
		// TODO part of Physics.cpp -> tick() -> step()
		mStartTime = std::chrono::high_resolution_clock::now();
		mLastUpdateTime = mStartTime;
		mAccumulator = std::chrono::duration<double>::zero();

		// Init physics
		world = physicsCommon.createPhysicsWorld();
		player.setSpawner(new GLGame::ObjectSpawner(physicsCommon, world, objects));
		player.setWorld(world);
		player.setCollider(GLGame::Collider(physicsCommon, world, glm::vec3(player.getCamera().Position)));

		//initDebug();

		// Shader simpleDepthShader("3.1.2.shadow_mapping_depth.vs", "3.1.2.shadow_mapping_depth.fs");

		// Select the contact points and contact normals to be displayed
		world->getDebugRenderer().setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);

		// Init objects

		GLGame::Object zombie = GLGame::Object(
			physicsCommon,
			world,
			glm::vec3(0.0f, 1.0f, 10.0f),
			BodyType::DYNAMIC,
			glm::vec3(0.5f, .2f, .5f),
			Model("../../../resources/zombie_w_anim.glb", glm::vec3(.0002f))
		);
		zombie.setTransformation(GLGame::Transformation(glm::radians(180.0f)));
		objects.push_back(zombie);

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
			glm::vec3(-21.5f, -1.0f, 10.0f),
			BodyType::STATIC,
			glm::vec3(10.0f, 0.2f, 10.0f),
			Model("../../../resources/FirstPersonMap.glb", glm::vec3(2.0f))
		));

		objects.push_back(GLGame::Object(
			physicsCommon,
			world,
			glm::vec3(21.5f, -3.0f, 10.0f),
			BodyType::STATIC,
			glm::vec3(10.0f, 0.2f, 10.0f),
			Model("../../../resources/FirstPersonMap.glb", glm::vec3(2.0f))
		));

		objects.push_back(GLGame::Object(
			physicsCommon,
			world,
			glm::vec3(0.0f, -3.5f, 31.0f),
			BodyType::STATIC,
			glm::vec3(10.0f, 0.2f, 10.0f),
			Model("../../../resources/FirstPersonMap.glb", glm::vec3(2.0f))
		));

		/*objects.push_back(GLGame::Object(
			physicsCommon,
			world,
			glm::vec3(-5.0f, -2.0f, 7.0f),
			BodyType::STATIC,
			glm::vec3(1.0f),
			Model("../../../resources/electrical_substation.glb", glm::vec3(1.5f))
		));*/

		/*objects.push_back(GLGame::Object(
			physicsCommon,
			world,
			glm::vec3(-3.0f, 1.0f, 12.0f),
			BodyType::STATIC,
			glm::vec3(1.0f),
			Model("../../../resources/low_poly_amulet_normal.glb", glm::vec3(1.5f))
		));

		GLGame::Object zombie2 = GLGame::Object(
			physicsCommon,
			world,
			glm::vec3(2.0f, 1.0f, 10.0f),
			BodyType::DYNAMIC,
			glm::vec3(0.5f, .2f, .5f),
			Model("../../../resources/zombie_another_anim.glb", glm::vec3(.0002f))
		);*/
		
		//zombie2.setTransformation(GLGame::Transformation(glm::radians(180.0f)));
		//objects.push_back(zombie2);
		

		mainShader.use();
		mainShader.setVec3("lightColor", 0.5f, 0.5f, 0.5f);
		mainShader.setVec3("lightDir", -0.5f, -0.5f, -0.5f);
		mainShader.setInt("shadowMap", 0);

		// configure depth map FBO
		// -----------------------
		
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// set GL_CLAMP_TO_BORDER and border color to prevent shadow map repeating
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (Status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void Scene::render()
	{
		camera = player.getCamera();
		player.processInput();

		/*debugShader.use();

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
		}*/

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

		/*glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)GLGame::Application::get().getWidth() / (float)GLGame::Application::get().getHeight(), 0.1f, 100.0f);
		debugShader.setMat4("projection", projection);
		
		glm::mat4 view = camera.GetViewMatrix();
		debugShader.setMat4("view", view);*/

		glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;

		glm::vec3 shadowMapCenter = camera.Position;
		// Distance to place light's eye so that shadowMapCenter is in the middle of the ortho projection's depth.
		float eyeDistanceFactor = G_SHADOW_ORTHO_NEAR_OFFSET + (G_SHADOW_BOX_DEPTH / 2.0f);
		glm::vec3 lightEyePosition = shadowMapCenter - G_LIGHT_DIRECTION * eyeDistanceFactor;

		lightView = glm::lookAt(lightEyePosition, shadowMapCenter, glm::vec3(0.0, 1.0, 0.0));

		lightProjection = glm::ortho(-G_SHADOW_ORTHO_WIDTH / 2.0f, G_SHADOW_ORTHO_WIDTH / 2.0f,
			-G_SHADOW_ORTHO_HEIGHT / 2.0f, G_SHADOW_ORTHO_HEIGHT / 2.0f,
			G_SHADOW_ORTHO_NEAR_OFFSET, G_SHADOW_ORTHO_FAR_OFFSET);

		lightSpaceMatrix = lightProjection * lightView;

		// render scene from light's point of view
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);		
		glClear(GL_DEPTH_BUFFER_BIT);

		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		
		player.draw(simpleDepthShader);
		for (GLGame::Object obj : objects) {
			obj.render(simpleDepthShader);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);		

		// reset viewport
		glViewport(0, 0, 1920, 1080);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		mainShader.use();

		//mainShader.setMat4("projection", projection);
		//mainShader.setMat4("view", view);
		// set light uniforms
		mainShader.setVec3("lightPos", lightPos);
		mainShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE0);


		player.draw(mainShader);

		for (GLGame::Object obj : objects) {
			obj.render(mainShader);
		}
		

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		/*debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);*/
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		

		
	}
}