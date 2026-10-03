#include "camera.hpp"
#include "components.hpp"
#include "gui_constants.hpp"
#include "entity.hpp" 

Camera::Camera(glm::vec3 cameraPos, glm::vec3 WorldUp, float Yaw, float Pitch, float Fov) {
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    pos = cameraPos;
    up = WorldUp;
    yaw_ = Yaw;
    pitch_ = Pitch;
    fov_ = Fov;
    speed = glm::vec3(0.0f);
}

glm::mat4 Camera::View() const {
    return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::Projection() const {
    return glm::perspective(glm::radians(fov_), GUIConstants::game_width / (float)GUIConstants::game_height, 0.1f, GUIConstants::render_distance);
}

void Camera::Move() {
    if (glm::length(speed) < 0.001f) {
        return; 
    }
    pos += speed;
    speed = glm::vec3(0.0f); 
}

void Camera::ProcessMouse(double xDiff, double yDiff) {
    yaw_ += float(xDiff) * SENS;
    pitch_ += float(yDiff) * SENS;
    
    if (pitch_ > 89.0f) {
        pitch_ = 89.0f;
    }
    if (pitch_ < -89.0f) {
        pitch_ = -89.0f;
    }
    
    float cos_yaw   = cos(glm::radians(yaw_));
    float cos_pitch = cos(glm::radians(pitch_));
    float sin_yaw   = sin(glm::radians(yaw_));
    float sin_pitch = sin(glm::radians(pitch_));

    glm::vec3 direction;
    direction.x = cos_yaw * cos_pitch;
    direction.y = sin_pitch;
    direction.z = sin_yaw * cos_pitch;
    front = glm::normalize(direction);
}

glm::vec3 Camera::GetPosition() const { return pos; }
void Camera::SetPosition(const glm::vec3& new_position) { pos = new_position; }