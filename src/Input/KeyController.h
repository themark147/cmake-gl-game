#pragma once

#include <vector>
#include <GLFW/glfw3.h>

namespace KeyInput {
    enum KeyType { // TODO naming KeyFunctionality ???
        HOLD,
        TOGGLE,
        PRESS,
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
        KEY_F = 8,
        KEY_SPACE = 9,
    };

    struct Key {
        int keyCode;
        bool state;
        int previousState;
        KeyType keyType;

        Key(int key, KeyType keyType = KeyType::HOLD) : keyCode(key),  state(false), previousState(GLFW_RELEASE), keyType(keyType) {}
    };

    class KeyController {
    public:
        void processKeys(GLFWwindow* window) {
            for (auto& key : keys) {
                int keyState = glfwGetKey(window, key.keyCode);

                // HOLD
                if (keyState == GLFW_PRESS && key.keyType == KeyType::HOLD) {
                    key.state = true;
                }
                else if (key.keyType == KeyType::HOLD) {
                    key.state = false;
                }

                // TOGGLE
                if (keyState == GLFW_PRESS && key.previousState == GLFW_RELEASE && key.keyType == KeyType::TOGGLE) {
                    key.state = !key.state;
                    printf("Key %d toggled to %s\n", key.keyCode, key.state ? "ON" : "OFF");
                }

                // PRESS
                if (keyState == GLFW_PRESS && key.previousState == GLFW_RELEASE && key.keyType == KeyType::PRESS) {
                    key.state = true;
                    key.previousState = GLFW_PRESS;
                }
                else if (key.previousState == GLFW_PRESS && key.keyType == KeyType::PRESS) {
                    key.state = false;
                }

                key.previousState = keyState;
            }
        }

        static KeyInput::KeyController& get() {
            static KeyController instance;
            
            return instance;
        }

        bool isKeyPressed(KeyDefinition keyDef) {
            return keys[static_cast<int>(keyDef)].state;
        }
    private:
        std::vector<KeyInput::Key> keys;

        KeyController::KeyController() {
            // It has to be in same order as KeyDefinition
            keys.push_back(Key(GLFW_KEY_G, KeyType::TOGGLE));
            keys.push_back(Key(GLFW_KEY_H, KeyType::TOGGLE));
            keys.push_back(Key(GLFW_KEY_T, KeyType::TOGGLE));
            keys.push_back(Key(GLFW_KEY_TAB, KeyType::TOGGLE));

            keys.push_back(Key(GLFW_KEY_W));
            keys.push_back(Key(GLFW_KEY_A));
            keys.push_back(Key(GLFW_KEY_S));
            keys.push_back(Key(GLFW_KEY_D));

            keys.push_back(Key(GLFW_KEY_F, KeyType::PRESS));
            keys.push_back(Key(GLFW_KEY_SPACE, KeyType::PRESS));
        }
    };
}
