#include "attack_system.hpp"

void AttackSystem::AttackPlayer(std::shared_ptr<PlayerGUI> player, std::vector<std::shared_ptr<Entity>> entites, float delta) {
    for (auto& entity: entites) {
        auto opposable = entity->GetComponent<Opposable>();
        auto transform = entity->GetComponent<Transform>();
        if (opposable && opposable->in_sight_ && transform && opposable->active) {
            opposable->enemy_->Tick(delta);
            if (glm::length(player->GetCamera().pos - transform->position) <= opposable->enemy_->GetRange()) {
                if (opposable->enemy_->CanAttack()) {

                    opposable->enemy_->is_attacking = true;
                    
                    player->GetPlayer()->DecreaseHP(opposable->enemy_->Attack());
                    player->hurt_signal = true;
                }
            }
        }   
    }
}
