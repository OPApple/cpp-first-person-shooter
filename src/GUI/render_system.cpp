#include "render_system.hpp"

void RenderSystem::Render(std::vector<std::shared_ptr<Entity>> entities, Camera& camera, std::shared_ptr<PlayerGUI> player, float time) {
        glm::mat4 view = camera.View();
        glm::mat4 projection = camera.Projection();

        for (auto& entity : entities) {
            auto transform = entity->GetComponent<Transform>();
            auto renderable = entity->GetComponent<Renderable>();
            if (transform && renderable && renderable->visible && renderable->IsValid()) {
                renderable->shader->Use();
                
                if (entity->HasComponent<Opposable>()) {
                    auto opp = entity->GetComponent<Opposable>();

                    if (!opp->enemy_->is_attacking) {

                        auto player_pos = player->GetEntity().GetComponent<Transform>()->position;
                        // Find out vector from entity to player, and compute the neccesary rotation
                        glm::vec3 ent_to_p   = glm::normalize(player_pos - transform->position);

                        float theta = glm::degrees(acos(glm::dot(ent_to_p, glm::vec3(0.0f, 0.0f, 1.0f))));

                        // handle angles > 180
                        if (ent_to_p.x < 0) {
                            theta = -theta;
                        }

                        transform->rotation = glm::vec3(0.0f, theta, 0.0f);
                    }  
                }
                // cool effect for pickups :)
                if (entity->GetName() == "Pickup") {
                    transform->position.y = 0.15f * sin(1.75f * time) + 0.85f;
                    transform->rotation.y = GUIConstants::pickup_rotation_speed * time;
                }
                renderable->shader->SetMat4("model", transform->GetModelMatrix());
                renderable->shader->SetMat4("view", view);
                renderable->shader->SetMat4("projection", projection);
                renderable->shader->SetVec3("viewPos", camera.pos);
                
                renderable->shader->SetInt("tex", 0);
                renderable->texture->Bind();
                renderable->mesh->Draw();
                renderable->texture->Unbind();
            }
        }
    }