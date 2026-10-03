#include "menu.hpp"
#include "gui_constants.hpp"
#include <iostream>
#include <memory>

Menu::Menu(float start_x, float start_y) : 
    start_x_(start_x), start_y_(start_y) {
        active_button = 0;
    }

void Menu::AddButton(std::shared_ptr<UIButton> btn) {
    if (buttons_.empty()) {
        btn->Select();
    }
    buttons_.push_back(btn);
}


const std::vector<std::shared_ptr<UIButton>> Menu::GetButtons() {
    return buttons_;
}

std::shared_ptr<UIButton> Menu::GetActive() {
    return buttons_[active_button];
}

void Menu::NextButton() {
    buttons_[active_button]->Deselect();
    ++active_button;
    if (active_button == int(buttons_.size())) {
        active_button = 0;
    }

    buttons_[active_button]->Select();
    
    
}

void Menu::PrevButton() {
    buttons_[active_button]->Deselect();
    --active_button;
    if (active_button < 0) {
        active_button = buttons_.size() - 1;
    }
    buttons_[active_button]->Select();
}

void Menu::Render(TextRenderer& text_renderer) {
    int buttons_rendered = 0;
    for (auto it: buttons_) {
        it->Draw(start_x_, start_y_ - (buttons_rendered * GUIConstants::button_offset), text_renderer);
        buttons_rendered++;
    }
}

Menu::~Menu() { }