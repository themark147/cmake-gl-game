#include "Scene.h"

#include "../Application/Application.h"
#include "../Model/Model.h"
#include "../Object/Object.h"
#include "../Shader.h"

#include <vector>
#include <iostream>

const unsigned int NUM_STRIPS = 2017 - 1;
const unsigned int NUM_VERTS_PER_STRIP = 2017 * 2;

int width, height, nChannels;
GLuint terrainVAO;

namespace GLGame {
	Scene::Scene() {
		reactphysics3d::PhysicsWorld* world = physics.getWorld();
		reactphysics3d::PhysicsCommon& common = physics.getPhysicsCommon();

		// Init player
		player.setSpawner(new GLGame::ObjectSpawner(world, objects));
		player.setWorld(world);
		player.setCollider(GLGame::Collider(common, world, glm::vec3(player.getCamera().Position)));

		initDebug();

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

		waves = new GLGame::Object(
			world,
			glm::vec3(0.0f, 3.0f, 0.0f),
			BodyType::STATIC,
			common.createBoxShape(Vector3(10.0f, 0.20f, 10.0f)),
			Model(std::string("plane.glb").insert(0, GLGame::RESOURCE_PATH), glm::vec3(5.0f))
		);

		mainShader.use();
		mainShader.setVec3("lightColor", 0.5f, 0.5f, 0.5f);
		mainShader.setVec3("lightDir", -0.5f, -0.5f, -0.5f);

		// LAST texture of shader
		mainShader.setInt("shadowMap", 15);

		// Init shadow map
		shadowMap.init();


		unsigned char* data = stbi_load(
			std::string("terrain.png").insert(0, GLGame::RESOURCE_PATH).c_str(),
			&width, &height, &nChannels, 0);
		// vertex generation
		std::vector<float> vertices;
		float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				// retrieve texel for (i,j) tex coord
				unsigned char* texel = data + (j + width * i) * nChannels;
				// raw height at coordinate
				unsigned char y = texel[0];

				// vertex
				vertices.push_back(-height / 2.0f + i);        // v.x
				vertices.push_back((int)y * yScale - yShift); // v.y
				vertices.push_back(-width / 2.0f + j );        // v.z
			}
		}

		stbi_image_free(data);

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < height - 1; i++)       // for each row a.k.a. each strip
		{
			for (unsigned int j = 0; j < width; j++)      // for each column
			{
				for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
				{
					indices.push_back(j + width * (i + k));
				}
			}
		}

		GLuint terrainVBO, terrainEBO;
		glGenVertexArrays(1, &terrainVAO);
		glBindVertexArray(terrainVAO);

		glGenBuffers(1, &terrainVBO);
		glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(float),       // size of vertices buffer
			&vertices[0],                          // pointer to first element
			GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &terrainEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(unsigned int), // size of indices buffer
			&indices[0],                           // pointer to first element
			GL_STATIC_DRAW);
		
	}
	
	void Scene::render()
	{
		camera = player.getCamera();
		player.processInput();

		if (physics.tick()) {
			player.processMovementInput();
		}

		// 1. Render to shadow map
		shadowMap.render(simpleDepthShader, camera.Position);
		
		player.draw(simpleDepthShader);
		for (GLGame::Object obj : objects) {
			obj.render(simpleDepthShader);
		}
		waves->render(simpleDepthShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);		

		// reset viewport
		glViewport(0, 0, GLGame::Application::get().getWidth(), GLGame::Application::get().getHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. Debug
		debugShader.use();

		// ----- Triangles ---- //
		const uint nbTriangles = physics.getWorld()->getDebugRenderer().getNbTriangles();

		if (nbTriangles > 0)
		{
			// Vertices
			mDebugVBOTrianglesVertices.bind();
			GLsizei sizeVertices = static_cast<GLsizei>(nbTriangles * sizeof(rp3d::DebugRenderer::DebugTriangle));
			mDebugVBOTrianglesVertices.copyDataIntoVBO(sizeVertices, physics.getWorld()->getDebugRenderer().getTrianglesArray(), GL_STREAM_DRAW);
			mDebugVBOTrianglesVertices.unbind();
		}

		int vertexPositionLoc = debugShader.getAttribLocation("aPos");
		// int vertexColorLoc = debugShader.getAttribLocation("vertexColor");

		// Triangles
		if (nbTriangles > 0) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			drawDebug(physics.getWorld()->getDebugRenderer(), vertexPositionLoc, 2);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)GLGame::Application::get().getWidth() / (float)GLGame::Application::get().getHeight(), 0.1f, 100.0f);
		debugShader.setMat4("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		debugShader.setMat4("view", view);

		// 3. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		mainShader.use();

		// mainShader.setMat4("projection", projection);
		// mainShader.setMat4("view", view);
		// set light uniforms
		glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
		mainShader.setVec3("lightPos", -2.0f, 4.0f, -1.0f);
		mainShader.setMat4("lightSpaceMatrix", shadowMap.getLightSpaceMatrix());

		player.draw(mainShader);

		for (GLGame::Object obj : objects) {
			obj.render(mainShader);
		}

		/*simpleWaterShader.use();

		simpleWaterShader.setMat4("u_projection", projection);
		simpleWaterShader.setMat4("u_view", view);

		simpleWaterShader.setFloat("u_time", glfwGetTime());

		simpleWaterShader.setFloat("u_waveAmplitude", 0.1f);
		simpleWaterShader.setFloat("u_waveFrequency", 5.0f);
		simpleWaterShader.setFloat("u_waveSpeed", 2.0f);

		simpleWaterShader.setVec3("u_viewPos", player.getCamera().Position);
		simpleWaterShader.setVec3("u_lightPos", glm::vec3(2.0f, 2.0f, 3.0f));
		simpleWaterShader.setVec3("u_lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		simpleWaterShader.setVec3("u_objectColor", glm::vec3(0.2f, 0.5f, 0.8f));		

		waves->render(simpleWaterShader);*/

		heightShader.use();

		heightShader.setMat4("projection", projection);
		heightShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		// lightingShader.setMat4("model", model * (*it)->getRotationMatrix());
		heightShader.setMat4("model", model);

		// draw mesh
		glBindVertexArray(terrainVAO);
		// render the mesh triangle strip by triangle strip - each row at a time
		for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
		{
			glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
				NUM_VERTS_PER_STRIP, // number of indices to render
				GL_UNSIGNED_INT,     // index data type
				(void*)(sizeof(unsigned int)
					* NUM_VERTS_PER_STRIP
					* strip)); // offset to starting index

			glBindVertexArray(0);
		}
	}
}