#include "ShadowMap.h"

#include <iostream>

const glm::vec3 G_LIGHT_SOURCE_ORIGINAL_POS(-2.0f, 4.0f, -1.0f);
const glm::vec3 G_LIGHT_DIRECTION = glm::normalize(glm::vec3(0.0f) - G_LIGHT_SOURCE_ORIGINAL_POS);
const float G_SHADOW_ORTHO_WIDTH = 20.0f;
const float G_SHADOW_ORTHO_HEIGHT = 20.0f;
const float G_SHADOW_BOX_DEPTH = 15.0f; // Desired depth of the shadow box
const float G_SHADOW_ORTHO_NEAR_OFFSET = 0.1f; // Near plane for ortho, relative to light's new eye
const float G_SHADOW_ORTHO_FAR_OFFSET = G_SHADOW_ORTHO_NEAR_OFFSET + G_SHADOW_BOX_DEPTH;

namespace GLGame {
	ShadowMap::ShadowMap() : shadowMapTexture(0), FBO(0), width(4096), height(4096) {}

	void ShadowMap::init() {

		// configure depth map FBO - init
		// -----------------------
		glGenFramebuffers(1, &FBO);
		glGenTextures(1, &shadowMapTexture);
		glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// set GL_CLAMP_TO_BORDER and border color to prevent shadow map repeating
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (Status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowMap::render(Shader& shader, glm::vec3 cameraPosition) {
		glm::mat4 lightProjection, lightView;

		glm::vec3 shadowMapCenter = cameraPosition;
		// Distance to place light's eye so that shadowMapCenter is in the middle of the ortho projection's depth.
		float eyeDistanceFactor = G_SHADOW_ORTHO_NEAR_OFFSET + (G_SHADOW_BOX_DEPTH / 2.0f);
		glm::vec3 lightEyePosition = shadowMapCenter - G_LIGHT_DIRECTION * eyeDistanceFactor;

		lightView = glm::lookAt(lightEyePosition, shadowMapCenter, glm::vec3(0.0, 1.0, 0.0));

		lightProjection = glm::ortho(-G_SHADOW_ORTHO_WIDTH / 2.0f, G_SHADOW_ORTHO_WIDTH / 2.0f,
			-G_SHADOW_ORTHO_HEIGHT / 2.0f, G_SHADOW_ORTHO_HEIGHT / 2.0f,
			G_SHADOW_ORTHO_NEAR_OFFSET, G_SHADOW_ORTHO_FAR_OFFSET);

		lightSpaceMatrix = lightProjection * lightView;

		// render scene from light's point of view
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, width, height);
		glClear(GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		// as last texture
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
		glActiveTexture(GL_TEXTURE0);
	}
};