#pragma once

#include <vector>

namespace KeyInput {
    enum KeyType { // TODO naming KeyFunctionality ???
        PRESS,
        TOGGLE
    };

    enum KeyDefinition {
        KEY_G = 0,
        KEY_H = 1,
        KEY_T = 2,
        KEY_TAB = 3,
        KEY_W = 4,
        KEY_A = 5,
        KEY_S = 6,
        KEY_D = 7,
    };

    struct Key {
        int keyCode;
        bool state;
        int previousState;
        KeyType keyType;

        Key(int key, KeyType keyType = KeyType::PRESS) : keyCode(key),  state(false), previousState(GLFW_RELEASE), keyType(keyType) {}
    };

    class KeyController {
    public:
        void processKeys(GLFWwindow* window, std::vector<Key> &keys) {
            for (auto& key : keys) {
                int keyState = glfwGetKey(window, key.keyCode);

                if (keyState == GLFW_PRESS && key.keyType == KeyType::PRESS) {
                    key.state = true;
                }
                else if (key.keyType == KeyType::PRESS) {
                    key.state = false;
                }

                if (keyState == GLFW_PRESS && key.previousState == GLFW_RELEASE && key.keyType == KeyType::TOGGLE) {
                    key.state = !key.state;
                    printf("Key %d toggled to %s\n", key.keyCode, key.state ? "ON" : "OFF");
                }

                key.previousState = keyState;
            }
        }
    };
}
