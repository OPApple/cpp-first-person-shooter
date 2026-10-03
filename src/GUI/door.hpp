#ifndef DOOR_HPP
#define DOOR_HPP

#include "wall.hpp"

/**
 * @brief Class for walls that can be opened
 */
class Door: public Wall {
public: 
    /**
     * @param grid_x x coordinate in map file grid
     * @param grid_y y coordinate in map file grid
     * @param face orientation of door
     */
    Door(int grid_x, int grid_y, Face face) : Wall(grid_x, grid_y, face) {};

    /**
     * @brief initializes components
     */
    virtual void Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture);

    //glm::vec3 pos_;
private: 
    //float rotation_;
};

#endif