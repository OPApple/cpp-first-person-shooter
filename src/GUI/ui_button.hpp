#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include <memory>
#include <string>
#include "text_renderer.hpp"
#include "scene.hpp"

/**
 * @brief Represents an interactive button element in a User Interface (UI).
 * * It stores the text to be displayed, a callback function to execute when pressed,
 * and its current active (selected) state.
 */
class UIButton {
public: 
    /**
     * @brief Constructs a UIButton.
     * @param text The string to be displayed on the button.
     * @param on_press A std::function callback that returns a new Scene pointer when the button is pressed.
     * @param activate Initial active (selected) state of the button. Defaults to false.
     */
    UIButton(const std::string& text, std::function<std::shared_ptr<Scene>()> on_press, bool activate=false);
    
    /**
     * @brief Draws the button text using the provided TextRenderer.
     * * * Renders the button differently based on its `active_` state (e.g., color and indentation changes).
     * @param x_pos The starting X-coordinate for rendering the text.
     * @param y_pos The starting Y-coordinate for rendering the text.
     * @param text_renderer A reference to the TextRenderer used for drawing.
     */
    void Draw(float x_pos, float y_pos, TextRenderer& text_renderer);

    /**
     * @brief Sets the button's state to selected (active).
     */
    void Select();

    /**
     * @brief Sets the button's state to deselected (inactive).
     */
    void Deselect();

    /**
     * @brief Executes the button's pressed action (the `on_press_` callback).
     * @return A shared pointer to the new Scene returned by the callback, or nullptr.
     */
    std::shared_ptr<Scene> Pressed();

    /**
     * @brief Destructor.
     */
    ~UIButton(); 
    
    std::string text_; ///< The text displayed on the button.
    
private:
    std::function<std::shared_ptr<Scene>()> on_press_;   ///< The callback function executed when the button is pressed.
    bool active_;                                        ///< True if the button is currently selected/active.
};

#endif