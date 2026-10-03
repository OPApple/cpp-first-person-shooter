#ifndef PLAYER_GUI_HPP
#define PLAYER_GUI_HPP

#include <memory>

#include "../Data/player.hpp"
#include "entity.hpp"
#include "camera.hpp"
#include "sound_handler.hpp"
#include "key_listener.hpp"

/**
 * @brief Represents the graphical and interactive player object in the game world.
 * * This class bridges the non-visual game logic (`Player` data) with the graphical
 * and input-handling elements (Camera, Entity, KeyListeners).
 */
class PlayerGUI {

private:
    std::shared_ptr<Player> player_;///< Shared pointer to the core player data object.
    Entity entity;///< The Entity component for collision and world representation.
    Camera camera;///< The Camera object representing the player's view.

    // KeyListeners for handling player input state (pressed, held, released)
    KeyListener forward_listener;
    KeyListener backward_listener;
    KeyListener left_listener;
    KeyListener right_listener;
    KeyListener reload_listener;
    KeyListener weapon_switch_listener;
    KeyListener one_listener;
    KeyListener two_listener;
    KeyListener space_listener;
    
    /**
     * @brief Checks if the player entity is currently moving.
     * @return true if the entity's Movable component has a significant velocity, false otherwise.
     */
    bool IsMoving();

    float move_speed; ///< Cached movement speed of the player.
    
public:
    /**
     * @brief Constructs a PlayerGUI object.
     * * * Initializes the internal Entity, Camera, and KeyListener objects.
     * * Creates and adds the necessary Transform, Collider, and Movable components to the Entity.
     * @param player A shared pointer to the underlying player data object.
     * @param start_pos The initial world position for both the camera and the entity.
     */
    PlayerGUI(std::shared_ptr<Player> player, const glm::vec3& start_pos);
    
    /**
     * @brief Default constructor.
     */
    PlayerGUI() = default;
    
    bool shoot_signal = false; ///< Flag indicating a shot was attempted and should be processed by another system.
    bool space_signal = false; ///< Flag indicating the 'action' key (E/Space) was pressed.
    bool hurt_signal = false; ///< Flag indicating the player has taken damage (currently unused in provided code).
    
    /** @brief Gets a reference to the internal Entity object. */
    Entity& GetEntity();

    /** @brief Gets a mutable reference to the internal Camera object. */
    Camera& GetCamera();

    /** @brief Gets a constant reference to the internal Camera object. */
    std::shared_ptr<Player> GetPlayer() const;

    /** @brief Gets a shared pointer to the core player data object. */
    const Camera& GetCamera() const;

    /**
     * @brief Processes keyboard and mouse button input for movement, actions, and shooting.
     * * * Updates the entity's velocity based on key presses (WASD).
     * * Handles weapon switching, reloading, and shooting, signaling other systems via flags.
     * * Updates the sound handler for footstep and shooting sounds.
     * @param window The GLFW window handle for checking input states.
     * @param deltaTime The time elapsed since the last frame.
     * @param sound_handler A reference to the sound handler for playing sounds.
     */
    void ProcessInput(GLFWwindow* window, float deltaTime, SoundHandler& sound_handler);

    /**
     * @brief Processes mouse movement input to update the camera's orientation (look direction).
     * @param xoffset The change in mouse position along the X-axis.
     * @param yoffset The change in mouse position along the Y-axis.
     */
    void ProcessMouse(float xoffset, float yoffset);

    /**
     * @brief Sets the camera's position to match the entity's position.
     * * * Ensures the player's view is always centered on their physical collision body.
     */
    void SyncCameraWithEntity();

    /**
     * @brief Sets the entity's position to match the camera's position.
     * * * Used after the camera has moved (e.g., via movement logic or teleportation).
     */
    void SyncEntityWithCamera();

    /**
     * @brief Sets the position of both the camera and the entity.
     * @param pos The new world position.
     */
    void SetPosition(glm::vec3 pos);

    /**
     * @brief Sets the internal cached movement speed.
     * @param speed The new movement speed value.
     */
    void SetMoveSpeed(float speed);

    /**
     * @brief Gets the internal cached movement speed.
     * @return The movement speed value.
     */
    float GetMoveSpeed() const;

};

#endif