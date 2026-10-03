#include "entity_handler.hpp"

EntityHandler::EntityHandler(Game& game) : game_(game) {
    player_ = PlayerGUI(game_.GetPlayers()[0], glm::vec3(0.0f, 1.5f, 3.0f));
    entities_.push_back(std::make_shared<Entity>(player_.GetEntity()));

    test_shader   = Shader("resources/shaders/shader.vs", "resources/shaders/shader.fs");
    test_texture = Texture("resources/textures/wall.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    test_mesh       = Mesh(GUIConstants::wall_vertices);
    
    enemy_mesh       = Mesh(GUIConstants::enemy_vertices);
    pickup_mesh      = Mesh(GUIConstants::pickup_vertices);

    floor_text = Texture("resources/textures/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    ceiling_text = Texture("resources/textures/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    
    default_texture = Texture("resources/textures/rui_seriously.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    floor_mesh = Mesh(GUIConstants::floorVertices);

    LoadEnemyTextures();
    LoadPickupTextures();
}

void EntityHandler::LoadEnemyTextures() {
    std::string textures_path = "resources/textures/enemies/";
    std::vector<std::string> paths;
    for (const auto & entry : std::filesystem::directory_iterator(textures_path)) {
        paths.push_back(entry.path().string());
    }
    
    for (auto path : paths) {
        std::string name = path.substr(textures_path.length(), path.size());
        name.erase(name.length() - 4); // remove file extension .png
        std::shared_ptr<Texture> tex = std::make_shared<Texture>(path.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        enemy_textures.insert(std::make_pair(name, tex));
    }
}

void EntityHandler::LoadPickupTextures() {
    std::string textures_path = "resources/textures/pickups/";
    std::vector<std::string> paths;
    for (const auto & entry : std::filesystem::directory_iterator(textures_path)) {
        paths.push_back(entry.path().string());
    }
    
    for (auto path : paths) {
        std::string name = path.substr(textures_path.length(), path.size());
        name.erase(name.length() - 4); // remove file extension .png
        std::shared_ptr<Texture> tex = std::make_shared<Texture>(path.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        pickup_textures.insert(std::make_pair(name, tex));
    }
}



std::vector<std::shared_ptr<Entity>> EntityHandler::GetEntites() {
    return entities_;
}

PlayerGUI& EntityHandler::GetPlayer() {
    return player_;
}

void EntityHandler::SetBoundaryTextures() {
    auto texture_legend = game_.GetLevel().GetTextureLegend();
    
    //set the floor texture
    auto tex = texture_legend.find('0');
    if (tex != texture_legend.end()) {
        const char* text_name = tex->second.c_str();
        floor_text = Texture(text_name, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    } else {
        floor_text = Texture("resources/textures/surfaces/wall.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    }
    
    //set the ceiling texture
    tex = texture_legend.find('c');
    if (tex != texture_legend.end()) {
        const char* text_name = tex->second.c_str();
        ceiling_text = Texture(text_name, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    } else {
        ceiling_text = Texture("resources/textures/surfaces/wall.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    }
};

void EntityHandler::Init() {

    SetBoundaryTextures();
    //create floor and ceiling for the area
    Entity floor("floor");
    floor.CreateAddComponent<Transform>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    floor.CreateAddComponent<Renderable>(std::make_shared<Shader>(test_shader), std::make_shared<Mesh>(floor_mesh), std::make_shared<Texture>(floor_text));
    entities_.push_back(std::make_shared<Entity>(floor));
    Entity ceiling("ceiling");
    ceiling.CreateAddComponent<Transform>(glm::vec3(0.0f, GUIConstants::wall_height, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    ceiling.CreateAddComponent<Renderable>(std::make_shared<Shader>(test_shader), std::make_shared<Mesh>(floor_mesh), std::make_shared<Texture>(ceiling_text));
    entities_.push_back(std::make_shared<Entity>(ceiling));

    //create the rest of the things read from level to the handler
    this->CreateEnemies();
    this->CreatePickups();
    this->CreateWalls();
}

void EntityHandler::CreateEnemies() {
    std::vector<std::shared_ptr<Enemy>> enemies = game_.GetLevel().GetEnemies();
    
    for (auto enemy : enemies) {
        auto gui_enemy = std::make_shared<EnemyGUI>(enemy, enemy->init_x, enemy->init_z);
        auto texture_it = enemy_textures.find(enemy->GetName());

        // Check if enemy has a texture loaded into enemy_textures
        if (texture_it == enemy_textures.end()) {
            gui_enemy->Init(
                std::make_shared<Shader>(test_shader), 
                std::make_shared<Mesh>(enemy_mesh), 
                std::make_shared<Texture>(default_texture)
            );
        } else {
            gui_enemy->Init(
                std::make_shared<Shader>(test_shader), 
                std::make_shared<Mesh>(enemy_mesh), 
                texture_it->second
            );
        }

        entities_.push_back(gui_enemy);
    }
}

void EntityHandler::CreatePickups() {
    std::vector<std::shared_ptr<Pickup>> pickups = game_.GetLevel().GetPickups();

    for (auto pickup : pickups) {
        auto gui_pickup = std::make_shared<PickupGUI>(pickup, pickup->init_x, pickup->init_z);
        auto texture_it = pickup_textures.find(pickup->GetName());
        // check the tex map
        if (texture_it == pickup_textures.end()) {
            gui_pickup->Init(
                std::make_shared<Shader>(test_shader),
                std::make_shared<Mesh>(pickup_mesh),
                std::make_shared<Texture>(default_texture)
            );
            std::cout << pickup->GetName() << std::endl;
        } else {
            gui_pickup->Init(
                std::make_shared<Shader>(test_shader),
                std::make_shared<Mesh>(pickup_mesh),
                texture_it->second
            );
        }

        entities_.push_back(gui_pickup);

        
    }
}

void EntityHandler::CreateWalls() {
    std::shared_ptr<Shader> wall_shader   = std::make_shared<Shader>("resources/shaders/shader.vs", "resources/shaders/shader.fs");
    std::shared_ptr<Mesh> wall_mesh       = std::make_shared<Mesh>(GUIConstants::wall_vertices);
    std::shared_ptr<Texture> wall_texture = nullptr;
    
    std::vector<std::string> map_grid = game_.GetLevel().GetMap();
    float s = GUIConstants::square_size;
    auto exit_legend = game_.GetLevel().GetExitLegend();
    auto texture_legend = game_.GetLevel().GetTextureLegend();
    for (size_t i = 0; i < map_grid.size(); ++i) {
        for (size_t j = 0; j < map_grid[i].size(); ++j) {

            char cur = map_grid[i][j];

            std::shared_ptr<Wall> new_wall = nullptr;
            std::shared_ptr<Door> new_door = nullptr;
            std::shared_ptr<Exit> new_exit = nullptr;
            
            auto tex = texture_legend.find(cur);
            if (tex != texture_legend.end()) {
                const char* text_name = tex->second.c_str();
                wall_texture = std::make_shared<Texture>(text_name, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
            }
            tex = texture_legend.find(cur);
            if (tex != texture_legend.end()) {
                const char* text_name = tex->second.c_str();
                wall_texture = std::make_shared<Texture>(text_name, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
            } else {
                wall_texture = std::make_shared<Texture>("resources/textures/wall.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
            }

            switch (cur) {
                case '^':
                    new_wall = std::make_shared<Wall>(j, i, NORTH);
                    break;
                case 'v':
                    new_wall = std::make_shared<Wall>(j, i, SOUTH);
                    break;
                case '<':
                    new_wall = std::make_shared<Wall>(j, i, WEST);
                    break;
                case '>':
                    new_wall = std::make_shared<Wall>(j, i, EAST);
                    break;
                case '1':
                    new_door = std::make_shared<Door>(j, i, NORTH);
                    break;
                case '2':
                    new_door = std::make_shared<Door>(j, i, SOUTH);
                    break;
                case '3':
                    new_door = std::make_shared<Door>(j, i, WEST);
                    break;
                case '4':
                    new_door = std::make_shared<Door>(j, i, EAST);
                    break;
                case 'P':
                    player_.SetPosition(glm::vec3(j * s, 1.5f, i * s));
                    continue;
                default:
                    // Check if its on the Exit legend
                    auto ex = exit_legend.find(cur);
                    if (ex != exit_legend.end()) {
                        new_exit = std::make_shared<Exit>(j, i, NORTH, ex->second);
                        break;
                    }
                    continue;
            }
            if (new_wall) {
                new_wall->Init(wall_shader, wall_mesh, wall_texture);
                entities_.push_back(new_wall);
            }
            if (new_door) {
                new_door->Init(wall_shader, wall_mesh, wall_texture);
                entities_.push_back(new_door);
            }

            if (new_exit) {
                new_exit->Init(wall_shader, wall_mesh, wall_texture);
                entities_.push_back(new_exit);
            }
        }
    }
}

EntityHandler::~EntityHandler() {
    test_shader.Del();
    test_texture.DelTexture();
}
