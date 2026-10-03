#include "door.hpp"

void Door::Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture) {

    this->CreateAddComponent<Renderable>(shader, mesh, texture);
    this->CreateAddComponent<Switchable>();
    this->CreateAddComponent<Interactable>(glm::vec3(2.0f, 0.5f, 0.5f), nullptr);
}