#include "key_listener.hpp"

KeyListener::KeyListener(int key) :
    key_(key) {}

bool KeyListener::IsPressed(GLFWwindow* window) {
    int new_state = -1;
    if (glfwGetKey(window, key_) == GLFW_PRESS) {
        new_state = GLFW_PRESS;
    }

    if(glfwGetKey(window, key_) == GLFW_RELEASE) {
        new_state = GLFW_RELEASE;
    }

    bool res = key_state_ == GLFW_PRESS && new_state == GLFW_RELEASE;

    key_state_ = new_state;

    return res;
}

bool KeyListener::IsHeld(GLFWwindow* window) {
    return glfwGetKey(window, key_) == GLFW_PRESS;
}
