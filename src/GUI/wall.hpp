#ifndef WALL_HPP
#define WALL_HPP

#include "entity.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "components.hpp"
#include "gui_constants.hpp"

/**
 * @brief Enumeration defining the four cardinal directions for a wall's placement relative to a grid cell.
 */
enum Face {
    NORTH, ///< Wall is on the negative Z boundary of the grid cell.
    EAST,  ///< Wall is on the positive X boundary of the grid cell.
    SOUTH, ///< Wall is on the positive Z boundary of the grid cell.
    WEST   ///< Wall is on the negative X boundary of the grid cell.
};

/**
 * @brief Represents a static wall entity in the game world, typically used for level geometry.
 * * Inherits from Entity and includes components for transformation, rendering, collision, and hit detection.
 */
class Wall: public Entity {
public: 
    /**
     * @brief Constructs a Wall entity.
     * * * Calculates the world position, rotation, and scale based on the given grid coordinates and face.
     * * Initializes the **Transform**, **Collider**, and **Hittable** components.
     * @param grid_x The X coordinate of the grid cell the wall belongs to.
     * @param grid_y The Y (Z-axis in world space) coordinate of the grid cell.
     * @param face The side of the grid cell on which the wall is placed.
     */
    Wall(int grid_x, int grid_y, Face face);

    /**
     * @brief Initializes the rendering components for the wall.
     * * Creates and adds the **Renderable** component using the provided visual assets.
     * @param shader A shared pointer to the shader program used for rendering.
     * @param mesh A shared pointer to the geometric mesh data.
     * @param texture A shared pointer to the texture applied to the wall.
     */
    virtual void Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture);

    glm::vec3 pos_; ///< The calculated world position of the wall.
private: 
    float rotation_; ///< The calculated rotation around the Y-axis.
};

#endif