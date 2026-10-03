#ifndef GUI_CONSTANTS_HPP
#define GUI_CONSTANTS_HPP

#include <vector>
#include "mesh.hpp"

namespace GUIConstants{

    const int game_width = 1920;
    const int game_height = 1080;

    //======================= Button and menu constants ==================================
    const float button_offset    = 75.0f;
    const float menu_text_height = game_height * 0.7f;
    const float menu_text_width  = game_width * 0.2f;
    const float menu_height      = game_height * 0.5f;
    const float save_slot_height = game_height * 0.2f;
    const float save_slot_width  = game_width * 0.25f;
    
    const float selected_button_indent    = 32.0f;
    const glm::vec3 button_color          = glm::vec3(0.0f, 0.0f, 0.0f);
    const glm::vec3 selected_button_color = glm::vec3(1.0f, 1.0f, 0.0f);
    const glm::vec3 menu_bg_color         = glm::vec3(0.1f, 0.2f, 1.0f);


    //=============================== Wall constants =======================================
    
    const float wall_height = 6.0f;

    const float square_size = 8.0f;


    //================================ Camera constants ===================================
    const float render_distance = 75.0f;


    //================================ HUD constants =======================================

    const glm::vec3 hp_color      = glm::vec3(0.9f, 0.0f, 0.1f);
    const glm::vec2 hp_pos        = glm::vec2(0.0f);
    const glm::vec2 weapon_pos    = glm::vec2(0.0f, 100.0f);
    const glm::vec2 status_pos    = glm::vec2(600.0f, 150.0f);
    const glm::vec2 reload_pos    = glm::vec2(game_width / 2 - 100.0f, game_height / 2 - 100.0f);
    const glm::vec2 crosshair_pos = glm::vec2(game_width / 2, game_height / 2);

    //================================ File constants ======================================
    //where save files are located
    const std::string save_folder_path = "resources/saves/";
    //where maps are located
    const std::string map_folder_path = "resources/maps/";
    //how many save can be made
    const unsigned int max_save_slots = 128;
    //============================== Pickup constants ======================================
    
    const float pickup_rotation_speed = 25.0f;

    
    static std::vector<Vertex> wall_vertices = {
        // positions          // tex coords
            // Back face
            {{-1.0f, -0.5f, -0.25f}, {0.0f, 0.0f}},
            {{ 1.0f, -0.5f, -0.25f}, {1.0f, 0.0f}},
            {{ 1.0f,  0.5f, -0.25f}, {1.0f, 1.0f}},
            {{ 1.0f,  0.5f, -0.25f}, {1.0f, 1.0f}},
            {{-1.0f,  0.5f, -0.25f}, {0.0f, 1.0f}},
            {{-1.0f, -0.5f, -0.25f}, {0.0f, 0.0f}},
    
            // Front face
            {{-1.0f, -0.5f,  0.25f}, {0.0f, 0.0f}},
            {{ 1.0f, -0.5f,  0.25f}, {1.0f, 0.0f}},
            {{ 1.0f,  0.5f,  0.25f}, {1.0f, 1.0f}},
            {{ 1.0f,  0.5f,  0.25f}, {1.0f, 1.0f}},
            {{-1.0f,  0.5f,  0.25f}, {0.0f, 1.0f}},
            {{-1.0f, -0.5f,  0.25f}, {0.0f, 0.0f}},
    
            // Left face
            {{-1.0f,  0.5f,  0.25f}, {1.0f, 0.0f}},
            {{-1.0f,  0.5f, -0.25f}, {1.0f, 1.0f}},
            {{-1.0f, -0.5f, -0.25f}, {0.0f, 1.0f}},
            {{-1.0f, -0.5f, -0.25f}, {0.0f, 1.0f}},
            {{-1.0f, -0.5f,  0.25f}, {0.0f, 0.0f}},
            {{-1.0f,  0.5f,  0.25f}, {1.0f, 0.0f}},
    
            // Right face
            {{ 1.0f,  0.5f,  0.25f}, {1.0f, 0.0f}},
            {{ 1.0f,  0.5f, -0.25f}, {1.0f, 1.0f}},
            {{ 1.0f, -0.5f, -0.25f}, {0.0f, 1.0f}},
            {{ 1.0f, -0.5f, -0.25f}, {0.0f, 1.0f}},
            {{ 1.0f, -0.5f,  0.25f}, {0.0f, 0.0f}},
            {{ 1.0f,  0.5f,  0.25f}, {1.0f, 0.0f}},
            
            // // floor 
            // {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
            // {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
            // {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
            // {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
            // {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
            // {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
            
            // // Roof
            // {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
            // {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
            // {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
            // {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
            // {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
            // {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}}
        };

    //=========================== Floor vertices ================================
    static std::vector<Vertex> floorVertices = {
        // Two triangles forming a square
        {{-300.0f, 0.0f, -300.0f}, {0.0f, 0.0f}},  // bottom-left
        {{ 300.0f, 0.0f, -300.0f}, {100.0f, 0.0f}},  // bottom-right
        {{ 300.0f, 0.0f,  300.0f}, {100.0f, 100.0f}}, // top-right
        {{ 300.0f, 0.0f,  300.0f}, {100.0f, 100.0f}}, // top-right
        {{-300.0f, 0.0f,  300.0f}, {0.0f, 100.0f}},  // top-left
        {{-300.0f, 0.0f, -300.0f}, {0.0f, 0.0f}}   // bottom-left
    };

    static std::vector<Vertex> enemy_vertices = {
        // Two triangles forming a square
        {{-0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f}},  // bottom-left
        {{ 0.5f,  -0.5f, 0.0f}, {1.0f, 0.0f}},  // bottom-right
        {{ 0.5f,   0.5f, 0.0f}, {1.0f, 1.0f}}, // top-right
        {{ 0.5f,   0.5f, 0.0f}, {1.0f, 1.0f}}, // top-right
        {{ -0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}},  // top-left
        {{-0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f}}   // bottom-left
    };

    static std::vector<Vertex> pickup_vertices = {
    // positions          // tex coords
        // Back face
        {{-0.5f, -0.5, -0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5, -0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5, -0.5f}, {0.0f, 0.0f}},

        // Front face
        {{-0.5f, -0.5,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5,  0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5,  0.5f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5,  0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5,  0.5f}, {0.0f, 0.0f}},

        // Left face
        {{-0.5f,  0.5,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5,  0.5f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5,  0.5f}, {1.0f, 0.0f}},

        // Right face
        {{ 0.5f,  0.5,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5,  0.5f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5,  0.5f}, {1.0f, 0.0f}},
        
        // // floor 
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
         {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        
        // Roof
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
         {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}}
    };

    //=========================== Arms rendering constants =========================================

    static std::vector<Vertex> arms_vertices = {
        // positions          // texCoords
        // Lower arm
        {{-0.1f, -0.1f,  0.0f},  {0.0f, 0.0f}},
         {{0.1f, -0.1f,  0.0f},  {1.0f, 0.0f}},
         {{0.1f,  0.5f,  0.0f},  {1.0f, 1.0f}},
        {{-0.1f,  0.5f,  0.0f},  {0.0f, 1.0f}},
        
        // Hand
        {{-0.15f, 0.5f,  0.0f},  {0.0f, 0.0f}},
         {{0.15f, 0.5f,  0.0f},  {1.0f, 0.0f}},
         {{0.15f, 0.7f,  0.0f},  {1.0f, 1.0f}},
        {{-0.15f, 0.7f,  0.0f},  {0.0f, 1.0f}}
    };

    static std::vector<unsigned int> arms_indices = {
        0, 1, 2,
        2, 3, 0
    };

    const float attack_duration = 0.3f;
    const float switch_duration = 0.3f;

    const float bob_speed = 8.0f;
    const float bob_amount = 0.05f;
    const float side_bob_amount = bob_amount * 0.6f;

    const float arms_image_x_scaling = 6.0f;
    const float arms_image_y_scaling = 2.0f;
    const float arms_y_pos = -0.85f;
};


#endif