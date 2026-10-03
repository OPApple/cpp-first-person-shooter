#include "pickup_system.hpp"


void PickupSystem::ResolvePickups(std::vector<std::shared_ptr<Entity>> entities, Camera& camera, std::shared_ptr<Player> player) {

    for (auto& entity : entities) {
        auto transform = entity->GetComponent<Transform>();
        auto renderable = entity->GetComponent<Renderable>();
        auto interactable = entity->GetComponent<Interactable>();

        if (transform && renderable && interactable && interactable->pickup_ && renderable->visible) {

            AABB aabb_i = interactable->GetWorldAABB(*transform);
            glm::vec3 cam_pos = camera.pos;
            if (aabb_i.ContainsPoint(cam_pos)) {
                renderable->visible = false;
                std::cout << interactable->pickup_->Effect(*player) << std::endl;
            }
        }
    }
}

void PickupSystem::ResolveSwitchable(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player, GameScene* gs) {

    for (auto& entity : entities) {
        auto transform = entity->GetComponent<Transform>();
        auto renderable = entity->GetComponent<Renderable>();
        auto interactable = entity->GetComponent<Interactable>();

        if (transform && renderable && interactable) {

            AABB aabb_i = interactable->GetWorldAABB(*transform);
            glm::vec3 player_pos = player->GetEntity().GetComponent<Transform>()->position;
            std::shared_ptr<Player> player_data = player->GetPlayer();
            if (aabb_i.ContainsPoint(player_pos)) {
                auto flag = entity->GetComponent<Flag>();
                auto switchable = entity->GetComponent<Switchable>();
                
                //if its a flag, start next
                if (flag && gs) {
                    std::cout << flag->next_level_.substr(flag->next_level_.size() - 7) << std::endl;
                    if (flag->next_level_.substr(flag->next_level_.size() - 7) == "victory") {
                        gs->won = true;
                    }
                    gs->level_name_ = flag->next_level_;
                    gs->Init();
                    //stop looping through entities when the next level should be loaded
                    break; 
                }
                
                //if there is a switchable component
                if (switchable) {
                    switchable->Toggle();
                    //toggle rendering
                    auto renderable = entity->GetComponent<Renderable>();
                    if (renderable) renderable->visible = switchable->GetState();
                    //toggle colliding
                    auto collider = entity->GetComponent<Collider>();
                    if (collider) collider->enabled = switchable->GetState();
                    std::cout << !switchable->GetState() << " toggled to " << switchable->GetState() << std::endl;
                }
            }
        }
    }
}