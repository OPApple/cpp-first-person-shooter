#include "ui_button.hpp"
#include "gui_constants.hpp"
#include <iostream>


UIButton::UIButton(const std::string& text, std::function<std::shared_ptr<Scene>()> on_press, bool activate) {
    text_ = text;
    on_press_ = on_press;
    active_ = activate;
    
}

void UIButton::Select() {
    active_ = true;
}

void UIButton::Deselect() {
    active_ = false;
}

std::shared_ptr<Scene> UIButton::Pressed() {
    return on_press_();
}

void UIButton::Draw(float x_pos, float y_pos, TextRenderer& text_renderer) {
    if (active_) {
        std::stringstream selected_text;
        selected_text << "-" << text_ << "-";
        text_renderer.render(selected_text.str(), x_pos - GUIConstants::selected_button_indent, y_pos, 2.0f, GUIConstants::selected_button_color);
    } else {
        text_renderer.render  (text_, x_pos, y_pos, 2.0f, GUIConstants::button_color);
    }
}

UIButton::~UIButton() { }