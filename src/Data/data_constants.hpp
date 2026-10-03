#ifndef DATA_CONSTANTS_HPP
#define DATA_CONSTANTS_HPP

//==== Player constants ====

const int basicHP = 100;
const int maxHP = 100;

const int basicHunger = 0;
const int maxHunger = 100;

const float basic_move_speed = 6.5f;

//==== Level constants ====

const int player_tile_weight = 0;
const int empty_tile_weight = 1;
const int heavier_tile_weight = 6;
const int wall_tile_weight = 1000;
const int maximum_path_weight = 1000;
const int default_dist_weight = 10000;

#endif