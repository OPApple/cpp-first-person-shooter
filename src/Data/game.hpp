#ifndef GAME_HPP
#define GAME_HPP

#include "level.hpp"
#include "player.hpp"
#include <memory>
#include <iostream>
#include <fstream>

/**
 * @brief The game data class contains methods to creating a level.
 * 
*/
class Game {
    public:
        Game();
        /** @brief Sets the level. */
        void SetLevel(Level new_level);
        /** @return Level of game. */
        Level GetLevel();
        /** @return Vector of pointers to players in the game. */
        std::vector<std::shared_ptr<Player>> GetPlayers();
        /** @brief Adds player to game. */
        void AddPlayer(std::shared_ptr<Player>);
        /** @brief Sets the current level based on a file stream. */
        void LoadLevel(const std::string& map_path);
    private:
        Level level_;
        std::vector<std::shared_ptr<Player>> players_ = std::vector<std::shared_ptr<Player>>(); //Change to actual Player class
};

#endif