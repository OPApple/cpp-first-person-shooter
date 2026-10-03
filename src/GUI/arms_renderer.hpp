#ifndef ARMS_RENDERER_HPP
#define ARMS_RENDERER_HPP

#include <glm/glm/glm.hpp>
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "gui_constants.hpp"

/**
 * class for handling weapon animations
 */
class ArmsRenderer {
public:

    enum class WeaponState {
        IDLE,
        ATTACKING,
        SWITCHING
        //TODO: RELOADING
    };

    ArmsRenderer();
    void Init();
    void Update(float delta_time, bool is_moving, bool is_attacking);

    void SetCurrentWeapon(const std::string& weapon_name);
    void Render();
    void Cleanup();

private:
    Shader arms_shader;
    Mesh arms_mesh;

    std::unordered_map<std::string, std::vector<Texture>> weapon_textures;
    std::string current_weapon;
    std::string next_weapon;

    WeaponState current_state = WeaponState::IDLE;

    float attack_progress = 0.0f;
    int current_frame = 0;

    float attack_timer = 0.0f; 
    float switch_timer = 0.0f; 

    float bob_time = 0.0f; 
    float bob_offset = 0.0f; 
    float bob_side_offset = 0.0f; 

};

#endif