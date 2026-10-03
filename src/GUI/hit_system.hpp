#ifndef HIT_SYSTEM_HPP
#define HIT_SYSTEM_HPP

#include <vector>
#include <memory>
#include "entity.hpp"
#include "components.hpp"
#include "player.hpp"
#include "game_scene.hpp"

/**
 * class for resolving player shooting and enemy sight
 */
class HitSystem {
public:
    static bool ResolveHits(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player);
    static void ResolveSight(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player);
};

#endif