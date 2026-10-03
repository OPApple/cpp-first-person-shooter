#ifndef ENTITY_HANDLER_HPP
#define ENTITY_HANDLER_HPP

#include <filesystem>
#include "enemy_gui.hpp"
#include "player.hpp"
#include "pickup_gui.hpp"
#include "../Data/game.hpp"
#include "gui_constants.hpp"
#include "wall.hpp"
#include "door.hpp"
#include "exit.hpp"


/**
 * @brief Handles entity creaton and storing
 */
class EntityHandler {
public:
    /**
     * @param game Game object used for Entity creation
     */
    EntityHandler(Game& game);

    ~EntityHandler();

    /**
     * @brief Creates all entities according to constructor parameter game
     */
    void Init();
    
    /**
     * @brief Sets floor and ceiling textures
     */
    void SetBoundaryTextures();

    /**
     * @brief Get current entity list
     * @return vector of Entity pointers
     */
    std::vector<std::shared_ptr<Entity>> GetEntites();

    /**
     * @brief Get current player 
     * @return current player as a PlayerGUI object
     */
    PlayerGUI& GetPlayer();

private:
    Game game_;
    PlayerGUI player_;
    std::vector<std::shared_ptr<Entity>> entities_;
    
    void CreatePlayers();
    void CreateEnemies();
    void CreatePickups();
    void CreateWalls();

    void LoadEnemyTextures();
    void LoadPickupTextures();
    
    Shader test_shader;
    Texture test_texture;
    Mesh test_mesh;
    Mesh enemy_mesh;
    Mesh pickup_mesh;
    Mesh floor_mesh;
    
    Texture floor_text;
    Texture ceiling_text;
    Texture default_texture;
    
    std::map<std::string, std::shared_ptr<Texture>> enemy_textures;
    std::map<std::string, std::shared_ptr<Texture>> pickup_textures;
};

#endif