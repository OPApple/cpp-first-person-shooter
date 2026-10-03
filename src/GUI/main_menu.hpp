#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "text_renderer.hpp"
#include "sound_handler.hpp"
#include "key_listener.hpp"
#include "scene.hpp"
#include "menu.hpp"


/**
 * @brief scene for main menu
 */
class MainMenu: public Scene {
public:
    MainMenu();
    virtual void Init();
    virtual std::shared_ptr<Scene> ProcessInput(GLFWwindow* window, SoundHandler& sound_handler);
    virtual void Render(TextRenderer& text_renderer, SoundHandler& sound_handler);

private: 
    KeyListener up_listener = KeyListener(GLFW_KEY_UP);
    KeyListener down_listener = KeyListener(GLFW_KEY_DOWN);
    KeyListener enter_listener = KeyListener(GLFW_KEY_ENTER);
    Menu menu_;
};

#endif