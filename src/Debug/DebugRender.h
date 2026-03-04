#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include "../Application/Application.h"
#include "../Shader.h"
#include "../Camera.h"

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

using namespace reactphysics3d;


namespace GLGame {
	class DebugRender {
	public:
		DebugRender();
		void Show(PhysicsWorld* world, Shader& shader, Camera& camera, glm::mat4 projection, glm::mat4 view);
	private:
		void Init();
		void Draw(DebugRenderer& debugRenderer, unsigned int vertexPositionLoc, unsigned int vertexColorLoc);

		// Vertex Buffer Object for the debug info lines vertices data
		openglframework::VertexBufferObject mDebugVBOLinesVertices = openglframework::VertexBufferObject(GL_ARRAY_BUFFER);

		/// Vertex Array Object for the lines vertex data
		openglframework::VertexArrayObject mDebugLinesVAO;

		/// Vertex Buffer Object for the debug info trinangles vertices data
		openglframework::VertexBufferObject mDebugVBOTrianglesVertices = openglframework::VertexBufferObject(GL_ARRAY_BUFFER);

		/// Vertex Array Object for the triangles vertex data
		openglframework::VertexArrayObject mDebugTrianglesVAO;
	};
}