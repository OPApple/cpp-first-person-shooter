#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "enemy.hpp"
#include "pickup.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <queue>


/** @brief The Level class is created by reading a file
 * @param enemy_legend_ Read from the beginning of the file. Example: <G, Enemy(10, 1.0f, 1.0f, 5, 1.0f, Sentry)>
 * @param pickup_legend_ Read from the beginning of the file. Example: <N, Hunger("Nuggets", 5);>
 * @param enemies_ List of all the enemy instances within the level.
 * @param pickups_ List of all the pickup instances within the level.
 */
class Level {
    public:
        Level();

        /**
         * @brief Loads all the necessary information from a file
         * @param filepath the full file path for the level file, example "resources/maps/first.txt"
         * @return The Level that is loaded from the given filepath.
         */
        static Level LoadFromFile(const std::string& filepath);

        /** WinMet() checks if the game if the level is done and the game
         *   can move onto the next one. */
        bool WinMet() const;

        std::vector<std::string> GetMap();
        std::vector<std::shared_ptr<Enemy>> GetEnemies();
        std::vector<std::shared_ptr<Pickup>> GetPickups();
        std::map<char, std::string> GetExitLegend() {return exit_legend_;}
        std::map<char, std::string> GetTextureLegend() {return texture_legend_;}
        std::string GetBGM() const {return bgm_;}

        /**
         * @brief Based on the characters in map_, crea
         * tes a map of vertex neighbour relations.
         * @return Void. Returns when done or map_ empty.
         */
        void set_map_vertex_neighbours();

        /**
         * @brief Based on the characters in map_, set weight at each vertice.
         * @return Void. Returns when done or map_ empty.
         */
        void set_map_weight_at();

        /**
         * @param start vertice where the algorythm starts 
         * @param end vertice where the algorythm should end 
         * @return queue of point points (std::queue<std::pair<int,int>>)
         */
        std::queue<std::pair<int,int>> ShortestPathFrom(std::pair<int,int> start, std::pair<int,int> end);
        
        /** Takes a point and return as a vertice */
        int point_to_vertice(std::pair<int,int> point);
        
    private:
        Level(std::vector<std::string>    map,
              std::map<char, std::string> enemy_legend, 
              std::map<char, std::string> pickup_legend, 
              std::map<char, std::string> exit_legend,
              std::map<char, std::string> texture_legend,
              std::string bgm);

        Level(std::vector<std::string>             map,
              std::vector<std::shared_ptr<Enemy>>  enemies, 
              std::vector<std::shared_ptr<Pickup>> pickups);

        std::vector<std::string>    map_;
        std::map<char, std::string> enemy_legend_ = std::map<char, std::string>();
        std::map<char, std::string> pickup_legend_ = std::map<char, std::string>();
        std::map<char, std::string> exit_legend_ = std::map<char, std::string>();
        std::map<char, std::string> texture_legend_ = std::map<char, std::string>();
        std::string bgm_ = "umg";

        std::vector<std::shared_ptr<Enemy>> enemies_ = std::vector<std::shared_ptr<Enemy>>();
        std::vector<std::shared_ptr<Pickup>> pickups_ = std::vector<std::shared_ptr<Pickup>>();
        
        std::map<int, std::vector<int>> map_vertex_neighbours{};
        std::vector<int> map_weight_at{};
        
};

#endif