cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(openglgame)

find_package(OpenGL REQUIRED)

set(RESOURCES_BINARY "${CMAKE_CURRENT_BINARY_DIR}/resources/")


file(GLOB SOURCES "src/**/*.cpp")
file(GLOB INCLUES2 "src/**/*.h")
set(SOURCE_FILES ${SOURCES} src/Main.cpp external/glad.c)
add_executable("${CMAKE_PROJECT_NAME}" ${SOURCE_FILES})

file(GLOB RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")
foreach(RESOURCE ${RESOURCE_FILES})
    configure_file(${RESOURCE} ${RESOURCES_BINARY} COPYONLY)
endforeach()

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(BUILD_SHARED_LIBS OFF)


add_subdirectory(lib/assimp)
add_subdirectory(lib/glm)
add_subdirectory(lib/glfw)
add_subdirectory(lib/reactphysics3d)

set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/reactphysics3d/include/")
include_directories(${INCLUDES})
include_directories(${INCLUES2})

# target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/*")

target_link_libraries("${CMAKE_PROJECT_NAME}" ${OPENGL_LIBRARY} glfw glm assimp ReactPhysics3D::reactphysics3d)