#include "arms_renderer.hpp"

ArmsRenderer::ArmsRenderer() {
}

void ArmsRenderer::Init() {
    arms_shader = Shader("resources/shaders/arms.vs", "resources/shaders/arms.fs");

    weapon_textures["Fist"] = {
        Texture("resources/textures/weapons/punch1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/punch2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/punch3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE)
    };

    weapon_textures["Pistol"] = {
        Texture("resources/textures/weapons/pistol1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/pistol2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/pistol3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/pistol4.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/pistol5.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE)
    };

    weapon_textures["Shotgun"] = {
        Texture("resources/textures/weapons/shotgun1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/shotgun2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/shotgun3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/shotgun4.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/shotgun5.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE)
    };

    weapon_textures["can"] = {
        Texture("resources/textures/weapons/can1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/can2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/weapons/can3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE)
    };
    
    current_weapon = "Fist";
    arms_mesh = Mesh(GUIConstants::arms_vertices, GUIConstants::arms_indices);
}

void ArmsRenderer::Update(float delta_time, bool is_moving, bool is_attacking) {

    if (current_state != WeaponState::SWITCHING && is_attacking) {
        current_state = WeaponState::ATTACKING;
        attack_timer = 0.0f;
        attack_progress = 0.0f;
    }

    switch (current_state) {
        case WeaponState::IDLE:
            if (is_moving) {
                bob_time += delta_time * GUIConstants::bob_speed;

                bob_offset = sin(bob_time) * GUIConstants::bob_amount;
                bob_side_offset = sin(bob_time * 0.7f + 1.0f) * GUIConstants::side_bob_amount;
            } else {
                bob_offset *= 0.9f;
                bob_time = 0.0f;
            }

            current_frame = 0;
            break;
        case WeaponState::ATTACKING:
            attack_timer += delta_time;
            attack_progress = attack_timer / GUIConstants::attack_duration;

            if (weapon_textures.find(current_weapon) != weapon_textures.end()) {
                int total_frames = weapon_textures[current_weapon].size();
                current_frame = static_cast<int>(attack_progress * total_frames);
                current_frame = std::min(current_frame, total_frames - 1);
            }
            
            if (attack_timer > GUIConstants::attack_duration) {
                current_state = WeaponState::IDLE;
                attack_progress = 0.0f;
                current_frame = 0;
            }
            break;
        case WeaponState::SWITCHING:
            switch_timer += delta_time;
            float switch_progress = switch_timer / GUIConstants::switch_duration;
            
            if (switch_progress < 0.5f) {
                current_frame = 0;
            } else {
                current_frame = 0;
            }
            
            if (switch_timer > GUIConstants::switch_duration) {
                current_weapon = next_weapon;
                current_state = WeaponState::IDLE;
                current_frame = 0;
            }
            break;
    }
}

void ArmsRenderer::SetCurrentWeapon(const std::string& weapon_name) {
    if (weapon_textures.find(weapon_name) != weapon_textures.end() && weapon_name != current_weapon) {
        next_weapon = weapon_name;
        current_state = WeaponState::SWITCHING;
        switch_timer = 0.0f;
    }
}

void ArmsRenderer::Render() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    arms_shader.Use();  
    Texture* texture_to_use = nullptr;
    
    if (current_state == WeaponState::SWITCHING) {
        float switch_progress = switch_timer / GUIConstants::switch_duration;
        
        if (switch_progress < 0.5f) {
            texture_to_use = &weapon_textures[current_weapon][current_frame];
        } else { 
            texture_to_use = &weapon_textures[next_weapon][current_frame];
        }
    } else {
        if (weapon_textures.find(current_weapon) != weapon_textures.end()) {
            texture_to_use = &weapon_textures[current_weapon][current_frame];
        }
    }
    
    if (texture_to_use) {
        texture_to_use->Bind();
    }

    glm::vec2 scale = glm::vec2(GUIConstants::arms_image_x_scaling, GUIConstants::arms_image_y_scaling);
    glm::vec2 pos = glm::vec2(0.0f, GUIConstants::arms_y_pos);
    
    if (current_state == WeaponState::SWITCHING) {
        float switch_progress = switch_timer / GUIConstants::switch_duration;
        float animation_offset = 0.0f;
        
        if (switch_progress < 0.5f) {
            animation_offset = -switch_progress * 2.0f; 
        } else {
            animation_offset = -1.0f + (switch_progress - 0.5f) * 2.0f; 
        }
        
        pos.y += animation_offset; 
    }
    
    if (current_state != WeaponState::SWITCHING) {
        pos.y += bob_offset;
        pos.x += bob_side_offset;
    }
    
    arms_shader.SetVec2("scale", scale);
    arms_shader.SetVec2("pos", pos);

    arms_mesh.Draw();
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void ArmsRenderer::Cleanup() {
    arms_shader.Del();
}