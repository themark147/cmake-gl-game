cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(openglgame)

find_package(OpenGL REQUIRED)

set(SOURCE_FILES src/Main.cpp external/glad.c)
add_executable("${CMAKE_PROJECT_NAME}" ${SOURCE_FILES})

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(BUILD_SHARED_LIBS OFF)

add_subdirectory(lib/assimp)
add_subdirectory(lib/glm)
add_subdirectory(lib/glfw)

set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/include/")
include_directories(${INCLUDES})

# target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/*")

target_link_libraries("${CMAKE_PROJECT_NAME}" ${OPENGL_LIBRARY} glfw glm assimp)