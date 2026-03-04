#include "DebugRender.h"

using namespace reactphysics3d;

namespace GLGame {
	DebugRender::DebugRender()
	{
		Init();
	}

	void GLGame::DebugRender::Init()
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

	void DebugRender::Show(PhysicsWorld* world, Shader& shader, Camera& camera, glm::mat4 projection, glm::mat4 view)
	{
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
			Draw(world->getDebugRenderer(), vertexPositionLoc, 2);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
	}

	void DebugRender::Draw(DebugRenderer& debugRenderer, unsigned int vertexPositionLoc, unsigned int vertexColorLoc)
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
}