#ifndef SOUND_HANDLER_HPP
#define SOUND_HANDLER_HPP

#include <SFML/Audio.hpp>
#include <map>
#include <iostream>
#include <filesystem>
#include <memory>

/**
 * @brief Class for handling all short sounds and background music (BGM) in the program using SFML Audio.
 * * It loads sounds at startup and provides simple methods to trigger sound effects (SFX) and manage music playback.
 * * Should be created once and passed to systems that need to play sounds.
 */
class SoundHandler {
public:

    /**
     * @brief Constructs a SoundHandler.
     * * * Initializes SFML sound buffers by loading audio files from predefined resource paths
     * (weapons, footsteps, menu) and sets up the default background music.
     * * Handles file loading errors gracefully.
     */
    SoundHandler();

    /** GAME SOUNDS */

    /**
     * @brief Plays the currently set player gunshot sound.
     */
    void PlayerShoot();
    
    /** @brief Changes the gunshot sound buffer to match the equipped weapon.
     * * Finds the corresponding `sf::SoundBuffer` in the internal map and attaches it to the player's gunshot sound object.
     * @param weapon_name name of the current weapon as a string (key into the internal map).
     */
    void SetPlayerShoot(std::string weapon_name);

    /**
     * @brief Handles footstep sound playback based on timing.
     * * * Plays the footstep sound only when a cooldown timer (player_step_timer) has elapsed,
     * ensuring the sound doesn't play every frame.
     * @param delta_time The time elapsed since the last frame, used to update the step timer.
     */
    void PlayerStep(float delta_time);

    /**
     * @brief Placeholder for enemy gunshot sound.
     */
    void EnemyShoot();


    /** MENU SOUNDS */

    /**
     * @brief Plays the menu "Enter" sound effect.
     */
    void Enter();

    /** MUSIC PLAYER */

    /**
     * @brief Starts playing the currently loaded background music (BGM).
     */
    void PlayBGM();

    /**
     * @brief Stops the current BGM and loads a new BGM file by name if it's different.
     * @param name The file name of the new BGM (without extension, e.g., "song_name").
     * @return true if a new BGM was successfully loaded and is different from the current one, false otherwise (e.g., file not found or already playing).
     */
    bool ChangeBGM(std::string name);

    
private:
    /** @brief Maps containing all loaded gunshot sound buffers. Key is the weapon name. */
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> gunshot_buffers;
    
    sf::SoundBuffer enter_buffer;           ///< Buffer for the menu "enter" sound.
    sf::SoundBuffer player_steps_buffer;    ///< Buffer for the player footstep sound.

    sf::Sound player_gunshot;               ///< The sound object for player shooting.
    sf::Sound player_step;                  ///< The sound object for player footsteps.
    sf::Sound enter;                        ///< The sound object for menu interaction.

    sf::Music bgm;                          ///< The music object for streaming background music.
    std::string bgm_name;                   ///< The name of the currently playing BGM file.

    float player_step_timer = 0.0f;         ///< Timer to regulate footstep sound frequency.
    
};

#endif