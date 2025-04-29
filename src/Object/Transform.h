#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace GLGame {
	struct Transformation {
		// Rotation
		float angle;
		glm::vec3 axis;

		Transformation(float angle = glm::radians(0.0f), glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f)) : angle(angle), axis(axis) {}
	};
}