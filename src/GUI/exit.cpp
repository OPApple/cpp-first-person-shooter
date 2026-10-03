#include "exit.hpp"

void Exit::Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture) {
    this->CreateAddComponent<Renderable>(shader, mesh, texture);
    this->CreateAddComponent<Flag>(level_name_);
    this->CreateAddComponent<Interactable>(glm::vec3(1.5f, 0.5f, 0.5f), nullptr);
}