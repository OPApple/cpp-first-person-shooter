#ifndef SCENE_HPP
#define SCENE_HPP

#include <glad/glad.h> //glad ENSIN!!!!
#include <GLFW/glfw3.h>
#include <memory>
#include "text_renderer.hpp"
#include "sound_handler.hpp"

/**  
   * @brief GUI class for representing different states of the main window.
   * If you wish to render something in the Window class, you must
   * inherit this class.
   *
*/
class Scene {
public:
    Scene(std::string bgm_path, glm::vec3 bg_color = glm::vec3(0.2f, 0.3f, 0.7f)): bgm_path_(bgm_path), bg_color_(bg_color) { }
    
    /**  
     * Define this for Scene derrivative classes
     * @param text_renderer Text renderer to be used in this scene
     * 
    */
    virtual void Render(TextRenderer& text_renderer, SoundHandler& sound_handler) = 0;

    /**  
     * Define this for Scene derrivative classes.
     * Is called on startup and when scene is changed.
     * @param text_renderer Text renderer for this scene
     */
    virtual void Init() = 0;

    /**
     * Define this for Scene derrivative classes.
     * 
     * @return shared_ptr<Scene> if scene should be changed or nullptr if scene should not be changed
     * @param window current glfw window
     * @param sound_handler sound handler of the window for playing reactive sounds
    */
    virtual std::shared_ptr<Scene> ProcessInput(GLFWwindow* window, SoundHandler& sound_handler) = 0;

    /**
     * Used in window to play different bgms for different scenes
     */
    const std::string GetBGM() const { 
        return bgm_path_; 
    }

    void SetBGM(std::string path) {
        bgm_path_ = path;
    }

    float GetRed() const {
        return bg_color_.x;
    }

    float GetGreen() const {
        return bg_color_.y;
    }

    float GetBlue() const {
        return bg_color_.z;
    }

private:
    std::string bgm_path_;
    glm::vec3 bg_color_;
};

#endif