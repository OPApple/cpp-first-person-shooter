#ifndef LOAD_MENU_HPP
#define LOAD_MENU_HPP

#include "scene.hpp"
#include "text_renderer.hpp"
#include "key_listener.hpp"
#include "menu.hpp"
#include <memory>

/**
 * @brief Scene for load files
 */
class LoadMenu: public Scene {
public:
    LoadMenu();
    
    virtual void Init();

    virtual std::shared_ptr<Scene> ProcessInput(GLFWwindow* window, SoundHandler& sound_handler);
    virtual void Render(TextRenderer& text_renderer, SoundHandler& sound_handler);

    unsigned int save_slot = 0;
private:
    
    KeyListener up_listener = KeyListener(GLFW_KEY_UP);
    KeyListener down_listener = KeyListener(GLFW_KEY_DOWN);
    KeyListener enter_listener = KeyListener(GLFW_KEY_ENTER);

    KeyListener left_listener    = KeyListener(GLFW_KEY_LEFT);
    KeyListener right_listener  = KeyListener(GLFW_KEY_RIGHT);
    Menu menu_;
};

#endif