#include "player.hpp"

PlayerGUI::PlayerGUI(std::shared_ptr<Player> player, const glm::vec3& start_pos = glm::vec3(0.0f)) : 
    player_(player), camera(start_pos)
{

    
    forward_listener = KeyListener(GLFW_KEY_W);
    backward_listener = KeyListener(GLFW_KEY_S);
    left_listener = KeyListener(GLFW_KEY_A);
    right_listener = KeyListener(GLFW_KEY_D);
    reload_listener = KeyListener(GLFW_KEY_R);
    weapon_switch_listener = KeyListener(GLFW_KEY_Q);
    one_listener = KeyListener(GLFW_KEY_1);
    two_listener = KeyListener(GLFW_KEY_2);
    space_listener = KeyListener(GLFW_KEY_E);

    move_speed = player_->GetMoveSpeed();
    
    entity = Entity("Tom");
    entity.CreateAddComponent<Transform>(
        glm::vec3(start_pos.x, 1.5f, start_pos.z),
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    );

    entity.CreateAddComponent<Collider>(glm::vec3(0.5f, 1.0f, 0.5f));
    entity.CreateAddComponent<Movable>(glm::vec3(0.0f), 2);
}

Entity& PlayerGUI::GetEntity() { return entity; }
Camera& PlayerGUI::GetCamera() { return camera; }
const Camera& PlayerGUI::GetCamera() const { return camera; }

std::shared_ptr<Player> PlayerGUI::GetPlayer() const {
    return player_;
}

void PlayerGUI::ProcessInput(GLFWwindow* window, float deltaTime, SoundHandler& sound_handler) {
    
    auto transform = entity.GetComponent<Transform>();
    auto movable = entity.GetComponent<Movable>();
    if (!transform || !movable) return;

    movable->velocity = glm::vec3(0.0f);

    float speed = player_->GetMoveSpeed();

    if (forward_listener.IsHeld(window)) {
        movable->velocity += camera.front * speed;
    }
    if (backward_listener.IsHeld(window)) {
        movable->velocity -= camera.front * speed;
    }
    if (right_listener.IsHeld(window)) {
        movable->velocity += glm::normalize(glm::cross(camera.front, camera.up)) * speed;
    }
    if (left_listener.IsHeld(window)) {
        movable->velocity -= glm::normalize(glm::cross(camera.front, camera.up)) * speed;
    }
    if (weapon_switch_listener.IsPressed(window)) {
        player_->NextWeapon();
        sound_handler.SetPlayerShoot(player_->GetCurrentWeapon()->GetName());
    }

    if(reload_listener.IsPressed(window)) {
        player_->Reload();
    }
    
    if(one_listener.IsPressed(window)) {
        player_->ChangeWeapon(1);
        sound_handler.SetPlayerShoot(player_->GetCurrentWeapon()->GetName());
    }
    
    if(two_listener.IsPressed(window)) {
        player_->ChangeWeapon(2);
        sound_handler.SetPlayerShoot(player_->GetCurrentWeapon()->GetName());
    }


    if(space_listener.IsPressed(window)) {
        space_signal = true;
    }
    
    player_->Tick(deltaTime);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
        if (player_->CanShoot()){
            sound_handler.PlayerShoot();
            shoot_signal = true;
            player_->Shoot();
        }
    }

    
    movable->velocity.y = 0.0f;
    camera.speed = movable->velocity * deltaTime;
    if (glm::length(camera.speed) > 0) {
        sound_handler.PlayerStep(deltaTime);
    }

    camera.Move();
    

    SyncCameraWithEntity();

}

void PlayerGUI::ProcessMouse(float xoffset, float yoffset) {
    camera.ProcessMouse(xoffset, yoffset);
}

void PlayerGUI::SyncCameraWithEntity() {
    
    auto transform = entity.GetComponent<Transform>();
    if (transform) {
        camera.SetPosition(transform->position);
    }
}

void PlayerGUI::SyncEntityWithCamera() {
    
    auto transform = entity.GetComponent<Transform>();
    if (transform) {
        transform->position = camera.GetPosition();
    }
}

void PlayerGUI::SetPosition(glm::vec3 pos) { 
    camera.pos = pos;
    SyncEntityWithCamera();    
}

bool PlayerGUI::IsMoving() {
    auto movable = entity.GetComponent<Movable>();
    return movable && glm::length(movable->velocity) > 0.1f;
}

void PlayerGUI::SetMoveSpeed(float speed) { move_speed = speed; }
float PlayerGUI::GetMoveSpeed() const { return move_speed; }