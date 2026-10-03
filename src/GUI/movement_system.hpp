#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include <vector>
#include "entity.hpp"
#include "player.hpp"
#include "enemy_gui.hpp"
#include "components.hpp"


class MovementSystem {

public:
    /** 
     * @brief Movement system function that gives all enemies the direction towards a player.
     * @param entities all the entities that might me enemies that need direction and velocities.
     * @param player the player targeted by all the enemies
     * @param delta_time
     */
    static void EnemyVelocity(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player);
    
    /**
     * @brief Moves entities towards their velocity.
     * @param entities List of entities to move.
     * @param delta_time
     */
    static void MoveEntities(std::vector<std::shared_ptr<Entity>> entities, float delta_time);
    
    /**
     * Handles pathfinding based on the given Level and PlayerGUI for all given entities that
     * have the Opposable component. If the Entity is not in sight of PlayerGUI, it gives the Opposable
     * a queue of points that it then moves towards.
     * @param level The Level in which the pathfinding takes place.
     * @param entities List of entities checked through.
     * @param player The PlayerGUI that the opposables seek a path to.
     * @param debug False by default. If true, prints lines with point and queue information.
     * @return Void
    */
    static void Pathfind(Level level, std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player, bool debug = false);
};


#endif