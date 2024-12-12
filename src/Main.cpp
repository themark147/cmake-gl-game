#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <std_image/std_image.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <reactphysics3d/reactphysics3d.h>

#include <iostream>
#include <vector>

#include "Camera.h"
#include "Object/Object.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

enum KeyDefinition {
    KEY_G = 0,
    KEY_H = 1,
    KEY_T = 2
};

struct Key {
    int keyCode;
    bool state;
    int previousState;

    Key(int key) : keyCode(key), state(false), previousState(GLFW_RELEASE) {}
};

std::vector<Key> keys;

Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
Object firstBox(glm::vec3(0.0f, 0.0f, 15.0f));

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // It has to be in same order as KeyDefinition
    keys.push_back(Key(GLFW_KEY_G));
    keys.push_back(Key(GLFW_KEY_H));
    keys.push_back(Key(GLFW_KEY_T));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        if (keys[KeyDefinition::KEY_T].state) {
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // 'T' key - Red
        }
        else if (keys[KeyDefinition::KEY_G].state) {
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // 'G' key - Green
        }
        else if (keys[KeyDefinition::KEY_H].state) {
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // 'H' key - Blue
        }
        else {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Default - Black
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    for (auto& key : keys) {
        int keyState = glfwGetKey(window, key.keyCode);

        if (keyState == GLFW_PRESS && key.previousState == GLFW_RELEASE) {
            key.state = !key.state;
            printf("Key %d toggled to %s\n", key.keyCode, key.state ? "ON" : "OFF");

        }

        key.previousState = keyState;
    }  
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}