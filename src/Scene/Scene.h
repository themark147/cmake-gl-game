#pragma once

#include <vector>
#include <reactphysics3d/reactphysics3d.h>

#include "../Object/Object.h"
#include "../Player/Player.h"

#include "../Shader.h"
#include "../Camera.h"
#include "../Model/Model.h"

#include "../Debug/VertexArrayObject.h"
#include "../Debug/VertexBufferObject.h"

#include "../ObjectSpawner.h"

using namespace reactphysics3d;

extern std::string vertexShaderMain, fragmentShaderMain,
	vertexShaderRender, fragmentShaderRender,
	vertexShaderDebug, fragmentShaderDebug,
	vertexShaderShadowMapping, fragmentShaderShadowMapping,
	vertexShaderDepthQuad, fragmentShaderDepthQuad;

namespace GLGame {
	class Scene {
	public:
		Scene();
		void render();

		PhysicsCommon& getPhysicsCommon() {
			return physicsCommon;
		}

		PhysicsWorld* getWorld() {
			return world;
		}
		
		GLGame::Player& getPlayer() {
			return player;
		}

		void initDebug()
		{
			mDebugVBOLinesVertices.create();

			// Create the VAO for both VBOs
			mDebugLinesVAO.create();
			mDebugLinesVAO.bind();

			// Bind the VBO of vertices
			mDebugVBOLinesVertices.bind();

			// Unbind the VAO
			mDebugLinesVAO.unbind();

			mDebugVBOLinesVertices.unbind();

			// ----- Triangles ----- //

			// Create the VBO for the vertices data
			mDebugVBOTrianglesVertices.create();

			// Create the VAO for both VBOs
			mDebugTrianglesVAO.create();
			mDebugTrianglesVAO.bind();

			// Bind the VBO of vertices
			mDebugVBOTrianglesVertices.bind();

			// Unbind the VAO
			mDebugTrianglesVAO.unbind();

			mDebugVBOTrianglesVertices.unbind();
		}

		void renderScene(const Shader& shader)
		{
			// floor
			glm::mat4 model = glm::mat4(1.0f);
			shader.setMat4("model", model);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// cubes
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, (glfwGetTime() / -5.0f) + 2.5f));
			model = glm::scale(model, glm::vec3(0.5f));
			shader.setMat4("model", model);
			renderCube();
		}


		// renderCube() renders a 1x1 3D cube in NDC.
		// -------------------------------------------------
		unsigned int cubeVAO = 0;
		unsigned int cubeVBO = 0;
		void renderCube()
		{
			// initialize (if necessary)
			if (cubeVAO == 0)
			{
				float vertices[] = {
					// back face
					-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
					 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
					 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
					 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
					-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
					-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
					// front face
					-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
					 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
					 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
					 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
					-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
					-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
					// left face
					-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
					-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
					-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
					-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
					-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
					-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
					// right face
					 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
					 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
					 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
					 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
					 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
					 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
					 // bottom face
					 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
					  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
					  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
					  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
					 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
					 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
					 // top face
					 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
					  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
					  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
					  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
					 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
					 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
				};
				glGenVertexArrays(1, &cubeVAO);
				glGenBuffers(1, &cubeVBO);
				// fill buffer
				glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				// link vertex attributes
				glBindVertexArray(cubeVAO);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			// render Cube
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		void renderQuad()
		{
			if (quadVAO == 0)
			{
				float quadVertices[] = {
					// positions        // texture Coords
					-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
					 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				};
				// setup plane VAO
				glGenVertexArrays(1, &quadVAO);
				glGenBuffers(1, &quadVBO);
				glBindVertexArray(quadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			}
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}

		void drawDebug(DebugRenderer& debugRenderer, unsigned int vertexPositionLoc, unsigned int vertexColorLoc)
		{
			// Bind the VAO
			mDebugTrianglesVAO.bind();

			mDebugVBOTrianglesVertices.bind();

			glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (char*)nullptr);
			glEnableVertexAttribArray(vertexPositionLoc);

			// glVertexAttribIPointer(vertexColorLoc, 3, GL_UNSIGNED_INT, sizeof(rp3d::Vector3) + sizeof(rp3d::uint32), (void*)sizeof(rp3d::Vector3));
			// glEnableVertexAttribArray(vertexColorLoc);

			 // Draw the triangles geometry
			glDrawArrays(GL_TRIANGLES, 0, debugRenderer.getNbTriangles() * 3);

			glDisableVertexAttribArray(vertexPositionLoc);
			//glDisableVertexAttribArray(vertexColorLoc);

			mDebugVBOTrianglesVertices.unbind();

			// Unbind the VAO
			mDebugTrianglesVAO.unbind();
		}
	private:
		PhysicsCommon physicsCommon;
		PhysicsWorld* world;
		glm::mat4 playerModelTransform;

		// TODO: object should contain model
		std::vector<Object> objects;
		std::vector<Model> models;

		GLGame::Player player = GLGame::Player(
			glm::vec3(0.0f, 0.0f, 15.0f),
			Model("../../../resources/first_person_arms_ue5_5.glb", glm::vec3(.02f))
		);

		Camera& camera = Camera();

		unsigned int planeVAO;

		Shader debugShader = Shader(vertexShaderDebug, fragmentShaderDebug);
		Shader mainShader = Shader(vertexShaderMain, fragmentShaderMain);
		Shader shadowMappingShader = Shader(vertexShaderShadowMapping, fragmentShaderShadowMapping);
		Shader depthQuadShader = Shader(vertexShaderDepthQuad, fragmentShaderDepthQuad);

		/// Vertex Buffer Object for the debug info lines vertices data
		openglframework::VertexBufferObject mDebugVBOLinesVertices = openglframework::VertexBufferObject(GL_ARRAY_BUFFER);

		/// Vertex Array Object for the lines vertex data
		openglframework::VertexArrayObject mDebugLinesVAO;

		/// Vertex Buffer Object for the debug info trinangles vertices data
		openglframework::VertexBufferObject mDebugVBOTrianglesVertices = openglframework::VertexBufferObject(GL_ARRAY_BUFFER);

		/// Vertex Array Object for the triangles vertex data
		openglframework::VertexArrayObject mDebugTrianglesVAO;
	};
}