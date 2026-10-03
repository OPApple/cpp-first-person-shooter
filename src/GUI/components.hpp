#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <glm/glm/glm.hpp>
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include <queue>
#include "../Data/pickup.hpp"
#include "../Data/enemy.hpp"
#include "../Data/level.hpp"


/**
 * @brief Axis aligned bounding box
 */
struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    
    AABB() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {}
    
    /**
     * @param min_point min point of AABB
     * @param max_point max point of AABB
     */
    AABB(glm::vec3 min_point, glm::vec3 max_point) 
        : min(min_point), max(max_point) {}
    
    /**
     * @brief Check if two AABBs intersect
     * @param other reference to other AABB
     * @return true if AABBs intersect, false otherwise
     */
    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
    
    /**
     * @brief Check if AABB contains given point
     * @param point point to be checked
     * @return true if AABB contains point, false otherwise
     */
    bool ContainsPoint(const glm::vec3& point) const {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }

    /**
     * @brief Get center of AABB
     * @return center of AABB as glm::vec3
     */
    glm::vec3 Center() const {
        return (min + max) * 0.5f;
    }
    
    /**
     * @brief Get size of AABB
     * @return size of AABB as glm::vec3
     */
    glm::vec3 Size() const {
        return max - min;
    }
};

/**
 * @brief Component containing position, rotation, scale and facing rule
 * of an Entity
 */
struct Transform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    bool face_player = false;


    Transform() = default; 

    /**
     * @param pos starting position of Entity
     * @param rot starting rotation of Entity
     * @param scl starting scale of Entity
     * @param face_player should Entity face the camera
     */
    Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, bool facePlayer=false) : 
        position(pos), rotation(rot), scale(scl), face_player(facePlayer) {}

    /**
     * @brief Computes the model matrix
     * @return model matrix as glm::mat4
     */
    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);

        return model;
    }

    /**
     * @brief Moves Entity according to velocity
     * @param velocity vector representing velocity
     */
    void AddSpeed(glm::vec3 velocity) {
        position += velocity;
    }
};

/**
 * @brief Component containing the shader, mesh and texture of an Entity
 */
struct Renderable {
    std::shared_ptr<Shader> shader = nullptr;
    std::shared_ptr<Mesh> mesh = nullptr;
    std::shared_ptr<Texture> texture = nullptr;
    bool visible = true;

    /**
     * @param sh shared_ptr to a Shader
     * @param m shared_ptr to a Mesh
     * @param tex shared_ptr to a texture
     */
    Renderable(std::shared_ptr<Shader> sh, std::shared_ptr<Mesh> m, std::shared_ptr<Texture> tex) : shader(sh), mesh(m), texture(tex) {}


    /**
     * @brief Checks if all necessary components are valid
     * @return true if rendering is possible, false otherwise
     */
    bool IsValid() const {
        return shader && mesh && texture;
    }
};


/**
 * @brief Component for collision checking
 */
struct Collider {
    glm::vec3 half_extents = glm::vec3(0.5f);
    bool enabled = true;

    Collider() = default;

    /**
     * @param he half extents of the collider
     */
    Collider(glm::vec3 he) : half_extents(he) {}

    /**
     * @brief Get the AABB in world coordinates
     * @param transform transfrom component of Entity
     * @return new AABB centered at transform's position 
     */
    AABB GetWorldAABB(const Transform& transform) const {
        glm::vec3 he = half_extents;

        // Build rotation matrix around Y only (walls are vertical)
        float yaw = glm::radians(transform.rotation.y);
        
        // Calculate the rotated half-extents
        glm::vec3 rotated_he;
        if (yaw == 0.0f || yaw == glm::radians(180.0f)) {
            // No rotation or 180° - keep original orientation
            rotated_he = he;
        } else {
            // 90° or 270° rotation - swap X and Z
            rotated_he = glm::vec3(he.z, he.y, he.x);
        }
        
        // Create AABB centered at position with the rotated half-extents
        return AABB(
            transform.position - rotated_he,
            transform.position + rotated_he
        );
    }
};


/**
 * @brief Component for moving entities
 */
struct Movable {
    glm::vec3 velocity = glm::vec3(0.1f);
    glm::vec3 target = glm::vec3(0.0f);
    float mass;

    Movable() = default;

    /**
     * @param vel starting velocity of Entity
     * @param mas mass of Entity
     */
    Movable(glm::vec3 vel, float mas) : velocity(vel), mass(mas) {}
};


struct Interactable {
    glm::vec3 half_extents = glm::vec3(0.5f);
    std::shared_ptr<Pickup> pickup_ = std::make_shared<Ammo>("Bungus", 1);
    bool enabled = true;

    Interactable() = default;
    Interactable(glm::vec3 ext, std::shared_ptr<Pickup> pickup) : half_extents(ext), pickup_(pickup) {}

    AABB GetWorldAABB(const Transform& transform) const {
        glm::vec3 scaledMin = -half_extents * transform.scale;
        glm::vec3 scaledMax = half_extents * transform.scale;
        return AABB(transform.position + scaledMin, transform.position + scaledMax);
    }
};

struct Hittable {
    std::shared_ptr<Enemy> enemy_ = nullptr;
    bool active_ = true;

    Hittable() = default;
    Hittable(std::shared_ptr<Enemy> enemy, bool active = true) : enemy_(enemy), active_(active) {}
};

/** 
 * @brief The component for an entity containing an enemy
 */
struct Opposable {
    std::shared_ptr<Enemy> enemy_; // = std::make_shared<Enemy>(0,0,0,0,0);
    bool active = true;
    std::queue<std::pair<int,int>> point_queue_;
    bool in_sight_ = false;
    Opposable() = default;
    
    /**
     * @param enemy shared pointer to an enemy
     */
    Opposable(std::shared_ptr<Enemy> enemy) : enemy_(enemy) {};

    float attack_duration = 0.5f;
    float attack_progress = 0.0f;

    /**
     * @brief Update attack animation
     * @param transform reference to Entity's transform
     * @param delta_time time change from last frame
     */
    void Update(Transform& transform, float delta_time) {
        if (enemy_->is_attacking) {
            attack_progress += delta_time;

            if (attack_progress <= attack_duration) {

                std::cout << "attacking is" << std::endl;

                float per_tick = glm::radians(45.0f / attack_duration);
                
                if ((attack_progress / attack_duration) < 0.5f) {
                    transform.rotation.y += per_tick;
                } else {
                    transform.rotation.y -= per_tick;
                }

            } else {
                attack_progress = 0.0f;
                enemy_->is_attacking = false;
            }
        }
    }
};


/** Component for things that can be switched on or off.
 * @param enabled tracks whether its on or off
 */
struct Switchable {
    bool enabled_ = false;

    Switchable() = default;

    /**
     * @brief Get current state
     * @return true if enabled, false otherwise
     */
    bool GetState() {
        return enabled_;
    }

    /** 
     * @brief Flips the value of enabled. 
    */
    void Toggle() {
        enabled_ = !enabled_;
    }
};

struct Flag {
    std::string next_level_ = "second";

    Flag() = default;
    Flag(std::string next_level) : next_level_(next_level) {};

};

#endif