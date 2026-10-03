#ifndef PICKUP_SYSTEM_HPP
#define PICKUP_SYSTEM_HPP

#include <vector>
#include "entity.hpp"
#include "camera.hpp"
#include "player.hpp"
#include "game_scene.hpp"

/**
 * @brief A system responsible for managing and resolving interactions with in-game pickup items and switchable objects.
 * * This system checks for collisions between the player/camera and Interactable components to trigger effects or state changes.
 */
class PickupSystem {

public:
    /**
     * @brief Resolves interactions with standard pickup items.
     * * * Checks if the camera's position (often used to represent the player's "eye" or center) is within the interaction bounds (AABB) of an entity that has a `Pickup` object.
     * * If an interaction occurs, the pickup's effect is applied to the player, and the pickup entity is hidden.
     * @param entities A vector of all entities in the scene.
     * @param camera The game camera, whose position is used for collision detection with pickups.
     * @param player The core non-GUI player data object to which the pickup's effect will be applied.
     */
    static void ResolvePickups(std::vector<std::shared_ptr<Entity>> entities, Camera& camera, std::shared_ptr<Player> player);

    /**
     * @brief Resolves interactions with switchable objects like flags (level exits) or toggles.
     * * * Checks if the player's position is within the interaction bounds (AABB) of an entity that has a `Switchable` or `Flag` component.
     * * If a `Flag` is hit, it triggers a level transition in the `GameScene`.
     * * If a `Switchable` is hit, it toggles its state and updates the entity's `Renderable` and `Collider` components accordingly.
     * @param entities A vector of all entities in the scene.
     * @param player The GUI player object, used to get the player's current world position.
     * @param gs A pointer to the current `GameScene`, needed to handle level transitions triggered by flags.
     */
    static void ResolveSwitchable(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player, GameScene*);
};


#endif