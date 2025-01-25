#include "Scene.h";

#include <reactphysics3d/reactphysics3d.h>

#include "../Object/Object.h"
#include "../Shader.h"
#include "../Application/Application.h"

#include <vector>

using namespace reactphysics3d;

namespace GLGame {
	Scene::Scene(Shader& shader) : shader(shader) {
		// Init physics
		world = physicsCommon.createPhysicsWorld();

		initDebug();

		// Select the contact points and contact normals to be displayed
		world->getDebugRenderer().setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);

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
		camera = player.getCamera();
		// TODO: inside physic step
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

		// TODO: get real aspect ratio
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)1920 / (float) 1080, 0.1f, 100.0f);
		shader.setMat4("projection", projection);
		
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);

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