#pragma once

//#ifndef GLAD_GLAD_H_
#include <glad/glad.h>
//#endif

#include <GLFW/glfw3.h>
#include <string>

namespace GLGame {
	const std::string RESOURCE_PATH = "../../../resources/"; // DEV
	// const std::string RESOURCE_PATH = "resources/"; // PROD

	const std::string SHADER_PATH = "../../../shaders/"; // DEV
	// const std::string SHADER_PATH = "shaders/"; // PROD
}