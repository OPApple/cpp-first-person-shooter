#ifndef EXIT_HPP
#define EXIT_HPP

#include "wall.hpp"

/**
 * @brief Wall that takes the player to a different level
 */
class Exit: public Wall {
public: 
    /**
     * @param grid_x x coordinate in the map file grid
     * @param grid_y y coordinate in the map file grid
     * @param face facing of the Exit
     * @param level_name name of target level
     */
    Exit(int grid_x, int grid_y, Face face, std::string level_name = "resources/maps/second.txt") :
        Wall(grid_x, grid_y, face) {level_name_ = level_name;};
    
    /**
     * @brief sets rendering assets
     */
    virtual void Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture);
private:
    std::string level_name_;
};

#endif