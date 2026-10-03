#include "enemy_gui.hpp"



EnemyGUI::EnemyGUI(std::shared_ptr<Enemy> enemy, int grid_x, int grid_z): Entity("enemy") {
    float x_pos = grid_x * GUIConstants::square_size;
    float z_pos = grid_z * GUIConstants::square_size;
    pos_ = glm::vec3(x_pos, 1.5f, z_pos);
    enemy_ = enemy;
}

void EnemyGUI::Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture) {
    this->CreateAddComponent<Transform>(pos_, glm::vec3(0.0f), glm::vec3(3.0f), true);
    this->CreateAddComponent<Renderable>(shader, mesh, texture);
    this->CreateAddComponent<Hittable>(enemy_);
    this->CreateAddComponent<Movable>(glm::vec3(0.0f), 1);
    this->CreateAddComponent<Collider>(glm::vec3(1.0f, 0.5f, 1.0f));
    this->CreateAddComponent<Opposable>(enemy_);
}
