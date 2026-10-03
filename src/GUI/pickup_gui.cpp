#include "pickup_gui.hpp"

PickupGUI::PickupGUI(std::shared_ptr<Pickup> pickup, int grid_x, int grid_z) : Entity("Pickup") {
    pickup_ = pickup;

    float x_pos = grid_x * GUIConstants::square_size;
    float z_pos = grid_z * GUIConstants::square_size;
    pos_ = glm::vec3(x_pos, 0.0f, z_pos);
}

void PickupGUI::Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture) {
    this->CreateAddComponent<Transform>(pos_, glm::vec3(0.0f), glm::vec3(0.5f));
    this->CreateAddComponent<Renderable>(shader, mesh, texture);
    this->CreateAddComponent<Interactable>(glm::vec3(2.0f), pickup_);
}