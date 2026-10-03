#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include "scene.hpp"

/**
 * @brief GUI class responsible for managing the main application window and the game loop.
 * * * It handles GLFW initialization, GLAD loading, OpenGL state setup, and scene management.
 * * FIXME: there has to be a better way to switch
 * scenes than the current implementation
 */
class Window {
public: 
    /**
     * @brief Constructs a Window object.
     * @param width The desired width of the window in pixels.
     * @param height The desired height of the window in pixels.
     */
    Window(int width, int height);

    /**
     * @brief Sets the currently active scene for the window.
     * @param new_scene A shared pointer to the Scene object that will be managed by the window.
     */
    void SetScene(std::shared_ptr<Scene> new_scene);

    /**
     * @brief Gets a shared pointer to the current active scene.
     * @return A shared pointer to the Scene.
     */
    std::shared_ptr<Scene> GetScene() const;

    /**
     * @brief Placeholder function for handling mouse input.
     * * The current implementation relies on static callbacks handled by the active scene.
     * @param window The GLFW window handle.
     * @param xpos The current x-coordinate of the mouse cursor.
     * @param ypos The current y-coordinate of the mouse cursor.
     */
    void MouseCallback(GLFWwindow* window, double xpos, double ypos);

    /**
     * @brief Initializes the GLFW window, sets up OpenGL, and starts the main game loop.
     * * The loop handles input processing, scene switching, rendering, and buffer swapping.
     */
    void Start();

private:
    int width_;                     ///< The width of the window.
    int height_;                    ///< The height of the window.
    GLFWwindow* glfw_window_;       ///< Pointer to the underlying GLFW window object.
    std::shared_ptr<Scene> scene_;  ///< The currently active game scene.
};

#endif