cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(openglgame)

find_package(OpenGL REQUIRED)


set(SOURCE_FILES src/Main.cpp include/glad.c)

add_executable("${CMAKE_PROJECT_NAME}" ${SOURCE_FILES})

set(GLFW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib")

set(GLFW_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/glfw3.h")
set(GLAD_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/glad.h")
set(KHR_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/KHW/khwplatform.h")
set(STD_IMAGE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/std_image/std_image.h")

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(BUILD_SHARED_LIBS OFF)

add_subdirectory(lib/assimp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/include/")

include_directories(
	${GLFW_INCLUDE_PATH}
	${GLEW_INCLUDE_PATH}
	${KHR_INCLUDE_PATH}
	${STD_IMAGE_INCLUDE_PATH}
)



add_subdirectory(thirdparty/glm)


# target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/*")

target_link_libraries("${CMAKE_PROJECT_NAME}" ${OPENGL_LIBRARY} ${GLFW_LIBRARY} glm assimp)