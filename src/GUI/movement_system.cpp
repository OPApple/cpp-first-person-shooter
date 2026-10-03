#include "movement_system.hpp"
#include <vector>
#include <queue>
#include <map>
#include "gui_constants.hpp"

void MovementSystem::EnemyVelocity(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player) {
    auto player_transform = player->GetEntity().GetComponent<Transform>();
    auto player_pos = player_transform->position;
    for (auto& entity : entities) {
        auto transform = entity->GetComponent<Transform>();
        auto movable = entity->GetComponent<Movable>();
        auto opposable = entity->GetComponent<Opposable>();
        if (transform && movable && opposable  && opposable->point_queue_.empty()) {
            auto enemy = opposable->enemy_;
            //check if player is not already close enough
            if (glm::distance(transform->position,player_pos) > enemy->GetRange()) {    
                switch (enemy->GetMoveType()) {
                    //run towards a point where the player was that refreshes randomly
                    case Bull:{
                            if (!(rand() % Enemy::bull_check_frame == 0)) {
                                break;
                            }
                            [[fallthrough]];
                        }
                        //No break! Bull does Chase behavior after if on bull_check_frame
                    //simple homing logic
                    case Chase:
                        {   //vector pointing from entity to player
                            auto direction = player_pos - transform->position;
                            //unit vector
                            auto unit_direction = glm::normalize(direction) * enemy->GetSpeed();
                            movable->velocity = unit_direction;
                            break;
                        }
                    //don't move and stand in place
                    case Sentry:
                        movable->velocity = glm::vec3(0,0,0);
                        break;
                }
            }
        }
    }
}

void MovementSystem::MoveEntities(std::vector<std::shared_ptr<Entity>> entities,  float delta_time) {
    for (auto& entity : entities) {
        auto transform = entity->GetComponent<Transform>();
        auto movable = entity->GetComponent<Movable>();

        if (transform && movable) {
            transform->AddSpeed(movable->velocity * delta_time);
        }
    }
}



void MovementSystem::Pathfind(Level level, std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<PlayerGUI> player, bool debug) {
    float s = GUIConstants::square_size;
    auto player_transform = player->GetEntity().GetComponent<Transform>();
    auto player_pos = player_transform->position;
    auto player_xz = std::make_pair(std::round(player_pos.x/s),std::round(player_pos.z/s));

    for (auto& entity : entities) {
        auto transform = entity->GetComponent<Transform>();
        auto movable = entity->GetComponent<Movable>();
        auto opposable = entity->GetComponent<Opposable>();
        
        //don't calculate if in sight of player
        if (opposable && opposable->in_sight_) {
            opposable->point_queue_ = std::queue<std::pair<int,int>>();
            break;
        }

        if (opposable && opposable->point_queue_.empty() && transform && movable) {
            auto my_pos = transform->position;
            auto my_xz = std::make_pair(std::round(my_pos.x/s),std::round(my_pos.z/s));
            //teleport if out of bounds
            if (my_xz.first < 0 || my_xz.second < 0) {
                transform->position = glm::vec3(2 * s, 1.5f, 2 * s);
                my_pos = transform->position;
                my_xz = std::make_pair(std::round(my_pos.x/s),std::round(my_pos.z/s));
            }
            if (debug) {
                std::cout << "from " << my_xz.first << " " << my_xz.second;
                std::cout << " re pathed to " << player_xz.first << " " << player_xz.second << std::endl;
            }
            opposable->point_queue_ = level.ShortestPathFrom(my_xz, player_xz);
            movable->velocity = glm::vec3(0,0,0);
        } else if (opposable && transform && movable) {
            //auto my_pos = transform->position;
            auto int_point = opposable->point_queue_.front();
            auto next_position = glm::vec3(int_point.first * s, 1.5f, int_point.second * s);
            //vector pointing from entity to player
            auto direction  = next_position - transform->position;
            if (debug) std::cout << "to " << next_position.x << " " << next_position.z << " left " << glm::distance(next_position, transform->position) << std::endl;
            //unit vector
            auto unit_direction = glm::normalize(direction) * opposable->enemy_->GetSpeed();
            movable->velocity = unit_direction;
            
            if (glm::distance(next_position, transform->position) < s * 0.2) {
                if (debug) std::cout << "popped " << int_point.first << " " << int_point.second << std::endl;
                opposable->point_queue_.pop();
            }
        }
    }
}