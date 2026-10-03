#include "wall.hpp"


Wall::Wall(int grid_x, int grid_y, Face face) : Entity("wall") {
    float width = GUIConstants::square_size;     // cell_size
    float height = GUIConstants::wall_height;    // wall_height  
    float thickness = height * 0.2f;
    
    glm::vec3 position;
    float rotation_y = 0.0f;
    
    switch (face) {
        case NORTH:
            position = glm::vec3(grid_x * width, height / 2.0f, grid_y * width - width/2.0f);
            rotation_y = 0.0f;
            break;
        case SOUTH:
            position = glm::vec3(grid_x * width, height / 2.0f, grid_y * width + width/2.0f);
            rotation_y = 0.0f;
            break;
        case WEST:
            position = glm::vec3(grid_x * width - width/2.0f, height / 2.0f, grid_y * width);
            rotation_y = 90.0f;
            break;
        case EAST:
            position = glm::vec3(grid_x * width + width/2.0f, height / 2.0f, grid_y * width);
            rotation_y = 90.0f;
            break;
    }
    
    // Scale: base mesh is 2x1x0.5, we want 6x3x0.6
    glm::vec3 scale(width / 2.0f, height, thickness * 2.0f); // 2*3=6, 1*3=3, 0.5*1.2=0.6
    
    this->CreateAddComponent<Transform>(position, glm::vec3(0.0f, rotation_y, 0.0f), scale);
    
    // Collider half-extents: half of final size (3x1.5x0.2)
    glm::vec3 half_extents(width / 2.0f, height / 2.0f, thickness / 2.0f);
    this->CreateAddComponent<Collider>(half_extents);

    this->CreateAddComponent<Hittable>();
    
}

void Wall::Init(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture) {

    this->CreateAddComponent<Renderable>(shader, mesh, texture);
}