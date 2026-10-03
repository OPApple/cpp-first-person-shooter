#ifndef KEY_LISTENER_HPP
#define KEY_LISTENER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @brief Class for listening to key presses
 */
class KeyListener {
public:
    // key should be a value from enum GLFW_KEY_XXX
    KeyListener(int key);

    KeyListener() = default;

    /**
     * @return true if pressed once, false otherwise
     */
    bool IsPressed(GLFWwindow* window);

    /**
     * @return true if held down, false otherwise
     */
    bool IsHeld(GLFWwindow* window);

private:
    int key_;
    GLFWwindow* window_;
    int key_state_ = 0;
    
};

#endif