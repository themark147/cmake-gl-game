#pragma once

#include "../Common/OpenGL.h"

#include "../Shader.h"

namespace GLGame {
	class ShadowMap {
	public:
		ShadowMap();

		void init();
		void render(Shader& shader, glm::vec3 cameraPosition);

		unsigned int getWidth() const { return width; }
		unsigned int getHeight() const { return height; }
		glm::mat4 getLightSpaceMatrix() const { return lightSpaceMatrix; }
	private:
		unsigned int shadowMapTexture;
		unsigned int FBO;
		unsigned int width;
		unsigned int height;

		glm::mat4 lightSpaceMatrix = glm::mat4(0.0f);
	};
}