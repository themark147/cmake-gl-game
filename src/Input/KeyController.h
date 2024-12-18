#ifndef KEYCONTROLLER_H
#define KEYCONTROLLER_H

#include <vector>

namespace KeyInput {
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

    class KeyController {
    public:
        void processKeys(GLFWwindow* window, std::vector<Key> &keys) {
            for (auto& key : keys) {
                int keyState = glfwGetKey(window, key.keyCode);

                if (keyState == GLFW_PRESS && key.previousState == GLFW_RELEASE) {
                    key.state = !key.state;
                    printf("Key %d toggled to %s\n", key.keyCode, key.state ? "ON" : "OFF");

                }

                key.previousState = keyState;
            }
        }
    };
}

#endif