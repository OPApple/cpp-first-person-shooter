#ifndef ENEMY_GUI_HPP
#define ENEMY_GUI_HPP

#include "entity.hpp"
#include "../Data/enemy.hpp"
#include "components.hpp"
#include "gui_constants.hpp"


/**
 * @brief Class for representing enemies in the GUI.
 */
class EnemyGUI: public Entity {
public: 
    /**
     * @param enemy reference to the enemy to be drawn.
     * @param x_pos Initial x position
     * @param z_pos Initial z position
     */
       EnemyGUI(std::shared_ptr<Enemy> enemy, int x_pos, int z_pos);

    /**
     * Sets texture, mesh and shaders
     */
    void Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture);

private:
    std::shared_ptr<Enemy> enemy_;
    glm::vec3 pos_;

};

#endif