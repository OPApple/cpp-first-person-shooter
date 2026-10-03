#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <vector>
#include "collision_system.hpp"

// Forward declaration only
class Entity;

/**
 * @brief Camera movement directions
 */
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float SENS = 0.05f;
const float CAMERA_RADIUS = 0.3f;

/**
 * @brief Class for controlling game camera
 */
class Camera {
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    float yaw_;
    float pitch_;
    glm::vec3 speed;
    float fov_;
    
    /**
     * @brief Has default values for all parameters
     * @param camera_pos starting position for camera
     * @param world_up upwards direction of world coordinates.
     * @param yaw starting yaw for camera in degrees
     * @param pitch starting pitch for camera in degrees
     * @param fov starting fov for camera
     */
    Camera(glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f), 
           glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f, float fov = 90.0f);
    
    /**
     * @brief Get the view matrix
     * @return view matrix as glm::mat4
     */
    glm::mat4 View() const;

    /**
     * @brief Get the projection matrix
     * @return projection matrix as glm::mat4
     */
    glm::mat4 Projection() const;
    
    /**
     * @brief enact camera movement. Called each frame to move the camera.
     */
    void Move();

    /**
     * @brief Changes camera speed according to keyboard input
     * @param dir direction of movement
     * @param move_speed movement speed of camera
     */
    void ProcessKeyBoard(CameraMovement dir, float move_speed);

    /**
     * @brief Changes camera front according to mouse input
     * @param x_diff mouse position x coordinate difference compared to last call
     * @param y_diff mouse position y coordinate difference compared to last call
     */
    void ProcessMouse(double x_diff, double y_diff);

    /**
     * @brief Get current position of camera
     * @return camera position as glm::vec3
     */
    glm::vec3 GetPosition() const;

    /**
     * @brief sets camera position to new_position
     * @param new_position new position for camera
     */
    void SetPosition(const glm::vec3& new_position);
    
private:
    glm::vec3 ResolveCollisions(const glm::vec3& desiredPos, std::vector<Entity>& collidableEntities);
};

#endif