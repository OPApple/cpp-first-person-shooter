#include "level.hpp"
#include <climits>
#include <map>
#include <algorithm>
#include <ranges>
#include "data_constants.hpp"

Level::Level() {};

Level::Level(std::vector<std::string> map,
    std::map<char, std::string> enemy_legend, 
    std::map<char, std::string> pickup_legend,
    std::map<char, std::string> exit_legend,
    std::map<char, std::string> texture_legend,
    std::string bgm
    )
{
    map_ = map;
    exit_legend_ = exit_legend;
    texture_legend_ = texture_legend;
    bgm_ = bgm;

    // Create pickups and enemies
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            char c = map[i][j];
            auto e_match  = enemy_legend.find(c);
            auto p_match = pickup_legend.find(c);

            if (e_match != enemy_legend.end()) {
                enemies_.push_back(std::make_shared<Enemy>(e_match->second, j, i));
            } else if (p_match != pickup_legend.end()) {
                pickups_.push_back(Pickup::CreatePickup(p_match->second, j, i));
            }
        }
    }

    set_map_vertex_neighbours();
    set_map_weight_at();

}

Level::Level(std::vector<std::string> map, std::vector<std::shared_ptr<Enemy>> enemies, std::vector<std::shared_ptr<Pickup>> pickups){
    map_ = map;
    enemies_ = enemies;
    pickups_ = pickups;
    set_map_vertex_neighbours();
    set_map_weight_at();
}

bool Level::WinMet() const {
    return false;
    //TODO:
};

Level Level::LoadFromFile(const std::string& filepath) {
    std::ifstream map_file(filepath);

    std::vector<std::string> map_grid;
    std::map<char, std::string> enemy_legend;
    std::map<char, std::string> pickup_legend;
    std::map<char, std::string> exit_legend;
    std::map<char, std::string> texture_legend;
    std::string bgm;

    std::string current;

    while (std::getline(map_file, current)) {
        // Check for ledger start
        if (current == "#### ENEMY LEGEND START ####") break;
        map_grid.push_back(current);
    }

    //get enemy legend
    while (std::getline(map_file, current)) {
        if (current == "#### PICKUP LEGEND START ####") break;
        char glyph = current[0];
        std::string enemy_name = current.substr(2, current.length());
        enemy_legend.insert(std::make_pair(glyph, enemy_name));
    }

    //get texture legend
    while (std::getline(map_file, current)) {
        if (current == "#### TEXTURE LEGEND START ####") break;
        char glyph = current[0];
        std::string pickup_name = current.substr(2, current.length());
        pickup_legend.insert(std::make_pair(glyph, pickup_name));
    }

    //get exit legend
    while (std::getline(map_file, current)) {
        if (current == "#### EXIT LEGEND START ####") break;
        char glyph = current[0];
        std::string texture_folder = "resources/textures/surfaces/";
        std::string texture_path = current.substr(2, current.length());
        texture_legend.insert(std::make_pair(glyph, texture_folder + texture_path));
    }

    //get 
    while (std::getline(map_file, current)) {
        if (current == "#### MUSIC ####") break;
        char glyph = current[0];
        std::string map_folder = "resources/maps/";
        std::string exit_name = current.substr(2, current.length());
        exit_legend.insert(std::make_pair(glyph, map_folder + exit_name));
    }

    while (std::getline(map_file, current)) {
        bgm = current;
    }

    return Level(map_grid, enemy_legend, pickup_legend, exit_legend, texture_legend, bgm);
};

std::vector<std::string> Level::GetMap() {
    return map_;
}

std::vector<std::shared_ptr<Enemy>> Level::GetEnemies() {
    return enemies_;
};

std::vector<std::shared_ptr<Pickup>> Level::GetPickups() {
    return pickups_;
};

void Level::set_map_weight_at() {
    if (map_.empty()) return;
    if (map_[0].empty()) return;
    int h = static_cast<int>(map_.size());
    int w = static_cast<int>(map_[0].size());
    int size = h*w;
    int vertice = 0;
    map_weight_at = std::vector<int>(size,-1);
    while (vertice < size) {
        int x = vertice%w;
        int y = vertice/w;


        switch (map_[y][x])
        {
        case '1':
            map_weight_at[vertice] = heavier_tile_weight;
            break;
        case '2':
            map_weight_at[vertice] = heavier_tile_weight;
            break;
        case '3':
            map_weight_at[vertice] = heavier_tile_weight;
            break;
        case '4':
            map_weight_at[vertice] = heavier_tile_weight;
            break;
        case '>':
            map_weight_at[vertice] = heavier_tile_weight;
            break;
        case '<':
            map_weight_at[vertice] = heavier_tile_weight;
            break;
        case '^':
            map_weight_at[vertice] = heavier_tile_weight;
            break;
        case 'v':
            map_weight_at[vertice] = heavier_tile_weight;
            break;
        case 'P':
            map_weight_at[vertice] = player_tile_weight;
            break;
        case 'L':
            map_weight_at[vertice] = wall_tile_weight;
            break;
        default:
            map_weight_at[vertice] = empty_tile_weight;
            break;
        }
        vertice++;
    }
};

void Level::set_map_vertex_neighbours() {
    if (map_.empty()) return;
    if (map_[0].empty()) return;
    int h = static_cast<int>(map_.size());
    int w = static_cast<int>(map_[0].size());
    int size = h*w;
    int vertice = 0;
    while (vertice < size) {
        int x = vertice%w;
        int y = vertice/w;
        char icon = map_[y][x];
        if (x > 0 && icon!='<')   map_vertex_neighbours[vertice].push_back(vertice-1);
        if (x < w-1 && icon!='>') map_vertex_neighbours[vertice].push_back(vertice+1);
        if (y > 0 && icon!='^')   map_vertex_neighbours[vertice].push_back(vertice-w);
        if (y < h-1 && icon!='v') map_vertex_neighbours[vertice].push_back(vertice+w);
        vertice++;
    }
}

/**
 * Dijkstra's algorythm based implementation for shortest path to a point with weigt 0.
 * @param nof_vertices number pf vertices to check
 * @param entry_spot vertice where the process starts
 * @param weight_at map of original weights
 * @param debug false by default. If true prints lines to help with debug.
 * @return list of vertices to target (std::vector<int>)
*/ 
std::vector<int> dijkstra(int nof_vertices, int entry_spot, std::map<int, std::vector<int>> vertex_neighbours, std::vector<int> weight_at, bool debug = false){
    /** @param Represents queue member */
    struct Road {
        int v; //vertex
        int w; //weight
        std::vector<int> r; //route here
         bool operator<(const Road& other) const
        {
            return w > other.w;
        }
    };

    std::vector<int> dist(nof_vertices, default_dist_weight); //Array of dist x to entry_spot for each vertex
    std::vector<int> path = std::vector<int>(); //The first path found

    std::priority_queue<Road> q;    //the great queue for all the thingies

    bool found = false;

    //initialize queue and the distance at entry_spot
    q.push(Road{entry_spot,1,std::vector<int>()});
    dist[entry_spot] = 1;

    while (!found && !q.empty()) {
        int vertex = q.top().v;
        int weight = q.top().w;
        auto route = q.top().r;
        q.pop(); //remove from queue after taking info
        if (debug) std::cout << "popping " << vertex << " w: " << weight << std::endl;

        if (weight == 0) {
            found = true;
            if (debug) std::cout << "route length " << route.size() << std::endl;
            path = route;
        }
        else {
            for (int other : vertex_neighbours[vertex]) {
                //get the weight and check if at goal
                int new_weight = weight_at[other] + weight;
                if (weight_at[other]==0) {
                    new_weight = 0;
                    if (debug) std::cout << "pushing goal" << std::endl;
                }

                if (dist[other] > new_weight) {
                    //when new distance is better, update distance and push onto queue
                    dist[other] = new_weight;
                    auto new_route = route;
                    new_route.push_back(other);
                    if (new_weight < maximum_path_weight) {
                        if (debug) std::cout << "pushing " << other << " w: " << new_weight << " from " << vertex << std::endl;
                        q.push(Road{other,new_weight,new_route});
                    } else if (debug) std::cout << "skipping " << other << " w: " << new_weight << " from " << vertex << std::endl;                    
                }
            }
        }
    }
    //return the final sequence of node ints
    return path;
}


std::queue<std::pair<int,int>> Level::ShortestPathFrom(std::pair<int,int> start, std::pair<int,int> end) {
    if (map_.empty() || (map_[0].empty()) || start == end) return std::queue<std::pair<int,int>>();
    int h = static_cast<int>(map_.size());
    int w = static_cast<int>(map_[0].size());
    int size = h*w;
    int nof_vertices = static_cast<int>(map_.size()*map_[0].size());
    std::queue<std::pair<int,int>> path;

    //reset target weights
    int vertice = 0;
    int end_v = point_to_vertice(end);
    while (vertice < size) {
        if (vertice == end_v) map_weight_at[vertice] = 0;
        else if (map_weight_at[vertice] == 0) map_weight_at[vertice] = 1;
        vertice++;
    }

    //find path nodes and add them to queue
    for (auto node : dijkstra(nof_vertices, point_to_vertice(start), map_vertex_neighbours, map_weight_at)) {
        path.push(std::make_pair(node%w, node/w));
    }
    return path;
};

int Level::point_to_vertice(std::pair<int,int> point) {
    int w = static_cast<int>(map_[0].size());
    return point.first + w * point.second;
}