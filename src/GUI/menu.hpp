#ifndef MENU_HPP
#define MENU_HPP

#include <memory>
#include <vector>
#include "ui_button.hpp"

/**  
 * @brief GUI class that describes a menu that consists of multiple buttons.
 */
class Menu {
public:
    /**
     * @param start_x x coordinate of menu start
     * @param start_y y coordinate of menu start
     */
    Menu(float start_x, float start_y);

    Menu() = default;

    /**
     * @brief adds a UIButton to menu
     * @param btn pointer to a UIButton
     */
    void AddButton(std::shared_ptr<UIButton> btn);

    /**
     * @return vector of UIButton pointers
     */
    const std::vector<std::shared_ptr<UIButton>> GetButtons();

    /**
     * @return pointer to currently selected pointer
     */
    std::shared_ptr<UIButton> GetActive();

    /**
     * @brief selects next button
     */
    void NextButton();

    /**
     * @brief selects previous button
     */
    void PrevButton();

    void Render(TextRenderer& text_renderer);

    ~Menu();

private:
    std::vector<std::shared_ptr<UIButton>> buttons_;
    int active_button; // active_button is an index in buttons_
    float start_x_;
    float start_y_;
};

#endif