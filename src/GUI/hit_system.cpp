#include "hit_system.hpp"

bool Compare(
    const std::pair<std::shared_ptr<Entity>, float>& pair_1, 
    const std::pair<std::shared_ptr<Entity>, float>& pair_2
) 
{
    return pair_1.second < pair_2.second;
}

bool HitSystem::ResolveHits(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player) {
    std::vector<std::pair<std::shared_ptr<Entity>, float>> hit_entities;
    
    glm::vec3 direction     = glm::normalize(player->GetCamera().front);
    glm::vec3 inv_direction  = 1.0f / direction;
    glm::vec3 player_pos    = player->GetCamera().pos;

    for (auto entity : entities) {
        auto hittable  = entity->GetComponent<Hittable>();
        auto transform = entity->GetComponent<Transform>();
        auto collider = entity->GetComponent<Collider>();

        if (hittable && transform && hittable->active_) {
            glm::vec3 max = collider->GetWorldAABB(*transform).max;
            glm::vec3 min = collider->GetWorldAABB(*transform).min;


            float tmin, tmax, tymin, tymax, tzmin, tzmax;

            if (inv_direction.x >= 0) { 
                tmin = (min.x - player_pos.x) * inv_direction.x;
                tmax = (max.x - player_pos.x) * inv_direction.x;
            } else { 
                tmin = (max.x - player_pos.x) * inv_direction.x;
                tmax = (min.x - player_pos.x) * inv_direction.x;
            }

            float y_offest = 3.0f;

            if (inv_direction.y >= 0) { 
                tymin = (min.y - y_offest - player_pos.y) * inv_direction.y;
                tymax = (max.y - player_pos.y) * inv_direction.y;
            } else { 
                tymin = (max.y - player_pos.y) * inv_direction.y;
                tymax = (min.y - y_offest - player_pos.y) * inv_direction.y;
            }            

            if ((tmin > tymax) || (tymin > tmax)) {
                continue;
            }

            if (tymin > tmin) tmin = tymin;
            if (tymax < tmax) tmax = tymax;

            if (inv_direction.z >= 0){ 
                tzmin = (min.z - player_pos.z) * inv_direction.z; 
                tzmax = (max.z - player_pos.z) * inv_direction.z;
            } else {
                tzmin = (max.z - player_pos.z) * inv_direction.z;
                tzmax = (min.z - player_pos.z) * inv_direction.z; 
            }


            if ((tmin > tzmax) || (tzmin > tmax)) {
                continue;
            }

            if (tzmin > tmin) tmin = tzmin; 
            if (tzmax < tmax) tmax = tzmax;

            float t = tmin;

            if (t < 0){
                t = tmax;
                if (t < 0) {
                    continue;
                }
            }

            auto hit_entity = std::make_pair(entity, tmin);
            hit_entities.push_back(hit_entity);

        }
    }

    if(!hit_entities.empty()) {
        std::sort(hit_entities.begin(), hit_entities.end(), Compare);
        std::shared_ptr<Entity> nearest = hit_entities[0].first;

        auto hittable = nearest->GetComponent<Hittable>();
        auto renderable = nearest->GetComponent<Renderable>();
        auto transform = nearest->GetComponent<Transform>();
        auto opposable = nearest->GetComponent<Opposable>();
        auto collider = nearest->GetComponent<Collider>();
        auto switchable = nearest->GetComponent<Switchable>();

        float dist = glm::length(player_pos - transform->position);
        
        if (hittable->enemy_ && dist <= player->GetPlayer()->GetCurrentWeapon()->GetRange()) {
            hittable->enemy_->TakeDamage(player->GetPlayer()->Damage());

            if (hittable->enemy_->Dead()) {
                renderable->visible = false;
                opposable->active = false;
                hittable->active_ = false;
                collider->enabled = false;
            }
            return true;
        } else {
            return false;
        }
        return true;

    }

    return false;
}


/** Checks which entities can see the player in question
 * @param entities the entities that might be able to see player
 * @param player the one towards whom sights are resolved
 */
void HitSystem::ResolveSight(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player) {
    bool debug = false;
    //for each opposable, check which hittables are between them and the player. Set in_sight if successful
    for (auto checking_entity : entities) {
        auto checking_opposable =  checking_entity->GetComponent<Opposable>();
        //if not checking distances for an enemy, return
        if (!checking_opposable) continue;
        //reset the opposables sight
        if (checking_opposable) {
            checking_opposable->in_sight_ = false;
            if (debug) std::cout << "nokaboo :8" << std::endl;
        }
        auto checking_transform = checking_entity->GetComponent<Transform>();
        //check the path in between
        glm::vec3 player_pos = player->GetEntity().GetComponent<Transform>()->position;
        glm::vec3 direction =  checking_transform->position - player_pos;
        std::vector<std::pair<std::shared_ptr<Entity>, float>> hit_entities;
        for (auto entity : entities) {
            auto hittable  = entity->GetComponent<Hittable>();
            auto transform = entity->GetComponent<Transform>();
            auto collider = entity->GetComponent<Collider>();
            //does the line between an opposable and the player collide
            if (hittable && transform && hittable->active_) {
                
                if (debug) std::cout << entity->GetName() << std::endl;

                glm::vec3 max = collider->GetWorldAABB(*transform).max;
                glm::vec3 min = collider->GetWorldAABB(*transform).min;

                float tmin, tmax, tymin, tymax, tzmin, tzmax;

                if (direction.x >= 0) { 
                    tmin = (min.x - player_pos.x) / direction.x;
                    tmax = (max.x - player_pos.x) / direction.x;
                } else { 
                    tmax = (min.x - player_pos.x) / direction.x;
                    tmin = (max.x - player_pos.x) / direction.x;
                }

                if (direction.y >= 0) { 
                    tymin = (min.y - player_pos.y) / direction.y;
                    tymax = (max.y - player_pos.y) / direction.y;
                } else { 
                    tymax = (min.y - player_pos.y) / direction.y;
                    tymin = (max.y - player_pos.y) / direction.y;
                }
                

                if ((tmin > tymax) || (tymin > tmax)) {
                    continue;
                }

                if (tymin > tmin) tmin = tymin;
                if (tymax < tmax) tmax = tymax;

                if (direction.z >= 0){ 
                    tzmin = (min.z - player_pos.z) / direction.z; 
                    tzmax = (max.z - player_pos.z) / direction.z;
                } else {
                    tzmax = (min.z - player_pos.z) / direction.z; 
                    tzmin = (max.z - player_pos.z) / direction.z;
                }


                if ((tmin > tzmax) || (tzmin > tmax)) {
                    continue;
                }

                if (tzmin > tmin) tmin = tzmin; 
                if (tzmax < tmax) tmax = tzmax; 

                auto hit_entity = std::make_pair(entity, glm::length(player_pos - transform->position));
                hit_entities.push_back(hit_entity);
            }
        }

        if (debug) std::cout << "found..." << std::endl;
        //if something was found
        if(!hit_entities.empty()) {
            std::sort(hit_entities.begin(), hit_entities.end(), Compare);
            //check each of the length sorted opposables until wall or list over
            for (auto i : hit_entities) {
                if (debug) std::cout << i.first->GetName() << std::endl;
                std::shared_ptr<Entity> nearest = i.first;

                auto hittable = nearest->GetComponent<Hittable>();
                auto opposable = nearest->GetComponent<Opposable>();
                //if the closest hittable is an actual opposable capable of seeing
                if (nearest == checking_entity) {
                    if (debug) std::cout << "peekaboo" << std::endl;
                    checking_opposable->in_sight_ = true;
                }
                else if (nearest->GetName()=="wall") {
                    if (debug) std::cout << "hit wall" << std::endl;
                    break;
                }
                else if (nearest->GetName()=="Tom") {
                    if (debug) std::cout << "why player??" << std::endl;
                    break;
                }
            }
        } else {
            if (debug) std::cout << "empty list for " << checking_entity->GetName() << std::endl;
        }
    }
};