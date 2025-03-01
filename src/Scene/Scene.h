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

extern std::string vertexShaderMain, fragmentShaderMain, vertexShaderRender, fragmentShaderRender;

namespace GLGame {
	class Scene {
	public:
		Scene(Shader& shader);
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

		// TODO: object should contain model
		std::vector<Object> objects;
		std::vector<Model> models;

		GLGame::Player player = GLGame::Player(glm::vec3(0.0f, 0.0f, 15.0f));
		Camera& camera = Camera();

		Shader shader;
		Shader mainShader = Shader(vertexShaderMain, fragmentShaderMain);

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