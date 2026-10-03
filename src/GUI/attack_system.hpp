#ifndef ATTACK_SYSTEM_HPP
#define ATTACK_SYSTEM_HPP

#include <vector>
#include "entity.hpp"
#include "player.hpp"
#include "../Data/enemy.hpp"

/**
 * @brief class for handling enemy attacks
 */
class AttackSystem {

public:
    static void AttackPlayer(std::shared_ptr<PlayerGUI> player, std::vector<std::shared_ptr<Entity>> entites, float delta);
};


#endif