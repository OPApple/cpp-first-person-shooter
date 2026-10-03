#include "main_menu.hpp"
#include "game_scene.hpp"
#include "key_listener.hpp"
#include "load_menu.hpp"

bool quit_flag = false;


std::shared_ptr<Scene> NewGameHandler() {
    return std::make_shared<GameScene>();
}

std::shared_ptr<Scene> QuitHandler() {
    quit_flag = true;
    return nullptr;
}


MainMenu::MainMenu(): Scene("Totally Boneless!!!", GUIConstants::menu_bg_color) {
    menu_ = Menu(GUIConstants::menu_text_width, GUIConstants::menu_height);
    
    menu_.AddButton(std::make_shared<UIButton>("New Game", NewGameHandler, true));
    menu_.AddButton(std::make_shared<UIButton>("Load Game", []{
        return std::make_shared<LoadMenu>();
    }));
    menu_.AddButton(std::make_shared<UIButton>("Quit Game", QuitHandler));

}

void MainMenu::Init() {
    return;
}

std::shared_ptr<Scene> MainMenu::ProcessInput(GLFWwindow* window, SoundHandler& sound_handler) {
    if (up_listener.IsPressed(window)) {
        menu_.PrevButton();
    }
    
    if (down_listener.IsPressed(window)) {
        menu_.NextButton();
    }
    
    if (enter_listener.IsPressed(window)) {
        sound_handler.Enter();
        return menu_.GetActive()->Pressed();
    }

    if (quit_flag) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    return nullptr;
}

void MainMenu::Render(TextRenderer& text_renderer, SoundHandler&) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    text_renderer.render("Menu!!", GUIConstants::menu_text_width, GUIConstants::menu_text_height, 3.0f, GUIConstants::button_color);
    menu_.Render(text_renderer);
}

