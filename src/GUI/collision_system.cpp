#include "collision_system.hpp"

CollisionSystem::CollisionInfo CollisionSystem::CheckPointAABB(const glm::vec3& point, const AABB& box, float radius) {
    CollisionInfo info;
    
    // find closest point on sphere center to AABB
    glm::vec3 closestPoint;
    closestPoint.x = glm::clamp(point.x, box.min.x, box.max.x);
    closestPoint.y = glm::clamp(point.y, box.min.y, box.max.y);
    closestPoint.z = glm::clamp(point.z, box.min.z, box.max.z);
    
    // calculate distance
    glm::vec3 difference = point - closestPoint;
    float distance = glm::length(difference);
    
    // check if there's a collision
    if (distance < radius) {
        info.collided = true;
        
        if (distance > 0.0f) {
            // get collision normal
            info.normal = glm::normalize(difference);
            info.penetration = info.normal * (radius - distance);
        } 
    }
    return info;
}

void CollisionSystem::ResolveCollisions(std::vector<std::shared_ptr<Entity>> entities) {
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            CollisionInfo collision = CheckCollisionAABB(entities[i], entities[j]);
            if (collision.collided) {
                ResolvePenetration(collision);
            }
        }
    }
}

CollisionSystem::CollisionInfo CollisionSystem::CheckCollisionAABB(std::shared_ptr<Entity> entity_a, std::shared_ptr<Entity> entity_b) {
    CollisionInfo info;
    info.entity_a = entity_a;
    info.entity_b = entity_b;

    auto transform_a = entity_a->GetComponent<Transform>();
    auto collider_a = entity_a->GetComponent<Collider>();

    auto transform_b = entity_b->GetComponent<Transform>();
    auto collider_b = entity_b->GetComponent<Collider>();

    if (transform_a && collider_a &&
        transform_b && collider_b &&
        collider_a->enabled && collider_b->enabled) {
            AABB aabb_a = collider_a->GetWorldAABB(*transform_a);
            AABB aabb_b = collider_b->GetWorldAABB(*transform_b);

            info.collided = aabb_a.Intersects(aabb_b);

            if (info.collided) {
                glm::vec3 center_a = aabb_a.Center();
                glm::vec3 center_b = aabb_b.Center();
                glm::vec3 direction = center_b - center_a;
                
                float overlap_x = std::min(aabb_a.max.x - aabb_b.min.x, aabb_b.max.x - aabb_a.min.x);
                float overlap_y = std::min(aabb_a.max.y - aabb_b.min.y, aabb_b.max.y - aabb_a.min.y);
                float overlap_z = std::min(aabb_a.max.z - aabb_b.min.z, aabb_b.max.z - aabb_a.min.z);
                
                if (overlap_x < overlap_y && overlap_x < overlap_z) {
                    info.normal = glm::vec3(direction.x > 0 ? -1.0f : 1.0f, 0.0f, 0.0f);
                    info.penetration = glm::vec3(overlap_x, 0.0f, 0.0f);
                } else if (overlap_y < overlap_z) {
                    info.normal = glm::vec3(0.0f, direction.y > 0 ? -1.0f : 1.0f, 0.0f);
                    info.penetration = glm::vec3(0.0f, overlap_y, 0.0f);
                } else {
                    info.normal = glm::vec3(0.0f, 0.0f, direction.z > 0 ? -1.0f : 1.0f);
                    info.penetration = glm::vec3(0.0f, 0.0f, overlap_z);
                }
            }   
        }
            
    return info;
}


void CollisionSystem::ResolvePenetration(const CollisionInfo& collision) {
    if (!collision.collided || !collision.entity_a || !collision.entity_b) {
        return;
    }

    Entity& entity_a = *collision.entity_a;
    Entity& entity_b = *collision.entity_b;

    auto transform_a = entity_a.GetComponent<Transform>();
    auto movable_a = entity_a.GetComponent<Movable>();
    auto transform_b = entity_b.GetComponent<Transform>();
    auto movable_b = entity_b.GetComponent<Movable>();

    if (!transform_a || !transform_b) {
        return;
    }

    // Calculate final penetration vector
    glm::vec3 final_penetration = collision.penetration * collision.normal;

    // Apply penetration based on which entities are movable
    if (movable_a && movable_b) {
        if (movable_a->mass < movable_b->mass) {
            transform_a->position += final_penetration;
            movable_a->velocity = glm::vec3(0.0f);
        } else {
            transform_a->position += final_penetration * 0.5f;
            transform_b->position -= final_penetration * 0.5f;
            movable_a->velocity = glm::vec3(0.0f);
            movable_b->velocity = glm::vec3(0.0f);
        }
    } else if (movable_a && !movable_b) {
        transform_a->position += final_penetration;
        movable_a->velocity = glm::vec3(0.0f);
    } else if (movable_b && !movable_a) {
        transform_b->position -= final_penetration;
        movable_b->velocity = glm::vec3(0.0f);
    } else {
        // Do nothing - both are static
    }
}