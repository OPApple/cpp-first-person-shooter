#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "scene.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "entity.hpp"
#include "key_listener.hpp"
#include "sound_handler.hpp"
#include "player.hpp"
#include "arms_renderer.hpp"
#include "../Data/level.hpp"
#include "../Data/enemy.hpp"
#include "../Data/game.hpp"
#include "../Data/player.hpp"
#include "entity_handler.hpp"
#include "menu.hpp"


/**
 * @brief scene for the main game
 */
class GameScene: public Scene {
public:
    GameScene();

    virtual void Init();

    virtual void Render(TextRenderer& text_renderer, SoundHandler& sound_handler);

    virtual std::shared_ptr<Scene> ProcessInput(GLFWwindow* window, SoundHandler& sound_handler);

    // FIXME: How do these work?
    static void StaticMouseCallback(GLFWwindow* window, double xpos, double ypos);

    void MouseCallback(GLFWwindow* window, double xpos, double ypos);

    void SetAsCurrentInstance();

    void DrawHud(TextRenderer& text_renderer);

    bool paused = false;
    bool won    = false;
    unsigned int save_slot = 0;

    void LoadSave(int index = 0);
    void SaveGameTo(int index = 0);

    std::string level_name_ = "resources/maps/smokinpiha.txt";

    

private:

    static GameScene* current_instance;

    Game game_;

    std::string last_weapon_name = "";

    std::unique_ptr<EntityHandler> entity_handler;

    std::unique_ptr<ArmsRenderer> arms_rend;

    std::shared_ptr<PlayerGUI> player;

    Menu dead_menu;
    Menu pause_menu;
    Menu victory_menu;

    KeyListener up_listener    = KeyListener(GLFW_KEY_UP);
    KeyListener down_listener  = KeyListener(GLFW_KEY_DOWN);
    KeyListener left_listener    = KeyListener(GLFW_KEY_LEFT);
    KeyListener right_listener  = KeyListener(GLFW_KEY_RIGHT);
    KeyListener enter_listener = KeyListener(GLFW_KEY_ENTER);
    KeyListener pause_listener = KeyListener(GLFW_KEY_ESCAPE);

};


#endif