#include "load_menu.hpp"
#include "main_menu.hpp"
#include "gui_constants.hpp"
#include "game_scene.hpp"

LoadMenu::LoadMenu(): Scene("Totally Boneless!!!", GUIConstants::menu_bg_color) {
    menu_ = Menu(GUIConstants::menu_text_width, GUIConstants::menu_height);
    
    
    menu_.AddButton(std::make_shared<UIButton>("Load Game", [&]{
        std::shared_ptr<GameScene> load_game = std::make_shared<GameScene>();
        load_game->LoadSave(save_slot);
        return load_game;
    }, true));

    menu_.AddButton(std::make_shared<UIButton>("Back", [&]{
        return std::make_shared<MainMenu>();
    }));

}

std::shared_ptr<Scene> LoadMenu::ProcessInput(GLFWwindow* window, SoundHandler&) {
    if (up_listener.IsPressed(window)) {
        menu_.PrevButton();
    }

    if (down_listener.IsPressed(window)) {
        menu_.NextButton();
    }

    if (enter_listener.IsPressed(window)) {
        return menu_.GetActive()->Pressed();
    }

    if (right_listener.IsPressed(window)) {
        if (save_slot < GUIConstants::max_save_slots) save_slot += 1;
    }

    if (left_listener.IsPressed(window)) {
        if (save_slot > 0) save_slot -= 1;
    }

    return nullptr;
}

void LoadMenu::Init() {
    return;
}

void LoadMenu::Render(TextRenderer& text_renderer, SoundHandler&) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::stringstream s_str;
    text_renderer.render(s_str.str(), GUIConstants::game_width / 3, GUIConstants::game_height * 0.8f, 2.0f, GUIConstants::hp_color);
    text_renderer.render("save slot: " + std::to_string(save_slot), GUIConstants::save_slot_width, GUIConstants::save_slot_height, 2.0f, GUIConstants::button_color);
    text_renderer.render("Load Game!", GUIConstants::menu_text_width, GUIConstants::menu_text_height, 5.0f, GUIConstants::button_color);
    menu_.Render(text_renderer);
}