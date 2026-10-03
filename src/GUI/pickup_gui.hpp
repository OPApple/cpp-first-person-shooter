#ifndef PICKUP_GUI_HPP
#define PICKUP_GUI_HPP

#include "entity.hpp"
#include "components.hpp"
#include "../Data/pickup.hpp"
#include "gui_constants.hpp"


/**
 * @brief Represents a visual, interactive pickup item in the game world.
 * * * This class inherits from Entity and serves as the graphical representation
 * and interaction point for a non-graphical `Pickup` data object.
 */
class PickupGUI: public Entity {
public:
    /**
     * @brief Constructs a PickupGUI object.
     * * * Initializes the entity's name and calculates its world position based on grid coordinates.
     * @param pickup A shared pointer to the underlying data object that defines what the pickup does.
     * @param grid_x The X-coordinate of the grid cell where the pickup is located.
     * @param grid_z The Z-coordinate of the grid cell where the pickup is located.
     */
    PickupGUI(std::shared_ptr<Pickup> pickup, int grid_x, int grid_z);

    /**
     * @brief Initializes the necessary components for rendering and interaction.
     * * * Creates and adds the Transform, Renderable, and Interactable components to the entity.
     * @param shader The shader program to be used for rendering.
     * @param mesh The geometric mesh data of the pickup.
     * @param texture The texture to be applied to the pickup's mesh.
     */
    void Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture);

private:
    glm::vec3 pos_;                   ///< The calculated world position of the pickup.
    std::shared_ptr<Pickup> pickup_;  ///< A pointer to the non-visual data object associated with this pickup.
};

#endif