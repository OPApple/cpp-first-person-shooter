#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP
#include <glm/glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "entity.hpp"
#include "components.hpp"

/**
 * class for collision detection
 */
class CollisionSystem {
public:
    //cool struct for returning collision status
    struct CollisionInfo {
        bool collided;
        glm::vec3 penetration;
        glm::vec3 normal;
        std::shared_ptr<Entity> entity_a = nullptr;
        std::shared_ptr<Entity> entity_b = nullptr;
        
        CollisionInfo() : collided(false), penetration(0.0f), normal(0.0f) {}
    };
    
    // Check collision between point (camera) and AABB
    static CollisionInfo CheckPointAABB(const glm::vec3& point, const AABB& box, float radius = 0.3f);
    static void ResolveCollisions(std::vector<std::shared_ptr<Entity>> entities);
    static CollisionInfo CheckCollisionAABB(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    static void ResolvePenetration(const CollisionInfo& collision);
};

#endif