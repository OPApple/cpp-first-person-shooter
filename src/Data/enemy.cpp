#include "enemy.hpp"
#include <iostream>
#include "pickup.hpp"

/**
 * @brief The enemy data class contains all the useful information for a given foe.
 * @param hp How much beating the creature can still take
 * @param range How from its target the crature must be to attack (unit: float distances)
 * @param attack_speed How fast the creature can atack again (unit: float seconds)
 * @param move_speed How much the creature moves atack again (unit: float distance moved in a tick)
 * @param move_type Describes the movement type for the creature.
*/
Enemy::Enemy(const std::string& name, int hp, float range, float attack_speed, int damage, float move_speed, MoveType move_type) :
    name_(name), hp_(hp), range_(range), attack_speed_(attack_speed), damage_(damage), move_speed_(move_speed), move_type_(move_type) {};

Enemy::Enemy(const std::string& name, int grid_x, int grid_y) {
    name_ = name;
    init_x = grid_x;
    init_z = grid_y;

    std::ifstream data_file("resources/data/enemies.txt");

    std::string current;

    // search for the correct enemy
    while(std::getline(data_file, current)) {
        if(current == name) break;
    }

    std::string cutoff = "end " + name;

    hp_           = IntForStat(data_file, "hp", cutoff);
    damage_       = IntForStat(data_file, "damage", cutoff);
    range_        = FloatForStat(data_file, "range", cutoff);
    attack_speed_ = FloatForStat(data_file, "attack_speed", cutoff);
    move_speed_   = FloatForStat(data_file, "move_speed", cutoff);
    move_type_    = static_cast<MoveType>(IntForStat(data_file, "move_type", cutoff));
}

std::string Enemy::GetName() const {
    return name_;
}

float Enemy::GetRange() const {
    return range_;
};

float Enemy::GetSpeed() const {
    return move_speed_;
};

MoveType Enemy::GetMoveType() const {
    return move_type_;
}

int Enemy::GetHp() const {
    return hp_;
}

bool Enemy::Alive() const {
    return hp_ > 0;
};
bool Enemy::Dead() const {
    return !Alive();
};
bool Enemy::CanAttack() const {
    return attack_timer_ >= attack_speed_;
};
int Enemy::Attack() {
    attack_timer_ = 0;
    return damage_;
};
void Enemy::Tick(float delta) {
    if (!CanAttack()) {
        attack_timer_ += delta;
    }
};
void Enemy::TakeDamage(int dmg) {
    hp_ -= dmg;
}