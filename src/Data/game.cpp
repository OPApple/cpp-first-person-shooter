#include "game.hpp"

Game::Game() : level_(Level()) {};

void Game::SetLevel(Level new_level) {
    level_ = new_level;
};

Level Game::GetLevel() {
    return level_;
};

std::vector<std::shared_ptr<Player>> Game::GetPlayers() {
    return players_;
};

void Game::AddPlayer(std::shared_ptr<Player> player) {
    players_.push_back(player);
}

void Game::LoadLevel(const std::string& map_path) {
    Level loaded_level = Level::LoadFromFile(map_path);
    level_ = loaded_level;
};