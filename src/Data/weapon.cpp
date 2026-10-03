#include "weapon.hpp"
#include "pickup.hpp"
#include <iostream>

Weapon::Weapon(std::string name, int currentAmmo, int maxAmmo, int stored_ammo, int damage, float attack_speed): 
name_(name), current_ammo_(currentAmmo), max_ammo_(maxAmmo), stored_ammo_(stored_ammo), damage_(damage), attack_speed_(attack_speed) {}

Weapon::Weapon(std::string name) {
    name_ = name;

    std::ifstream data_file("resources/data/weapons.txt");

    std::string current;

    // search for the correct enemy
    while(std::getline(data_file, current)) {
        if(current == name) break;
    }
    std::string cutoff = "end " + name;
    current_ammo_ = IntForStat(data_file, "current_ammo", cutoff);
    stored_ammo_  = IntForStat(data_file, "stored_ammo", cutoff);
    max_ammo_     = IntForStat(data_file, "max_ammo", cutoff);
    damage_       = IntForStat(data_file, "damage", cutoff);
    attack_speed_ = FloatForStat(data_file, "attack_speed", cutoff);
    reload_speed_ = FloatForStat(data_file, "reload_speed", cutoff, 1.0f);
    range_ = FloatForStat(data_file, "range", cutoff, 1.0f);
}


int Weapon::GetCurrentAmmo() {
    return current_ammo_;
}
int Weapon::GetMaxAmmo() {
    return max_ammo_;
}
int Weapon::GetStoredAmmo() {
    return stored_ammo_;
}

int Weapon::GetDamage() {
    return damage_;
}
std::string Weapon::GetName() {
    return name_;
}
float Weapon::GetAttackSpeed() const {
    return attack_speed_;
}

float Weapon::GetReloadSpeed() const {
    return reload_speed_;
}

float Weapon::GetRange() const {
    return range_;
}

void Weapon::IncreaseStored(int amount) {
    stored_ammo_ += amount;
}

void Weapon::DecreaseCurrentAmmo(int amount) {
    current_ammo_ -= amount;
}

void Weapon::Reload() {
    int diff = max_ammo_ - current_ammo_;
    if (stored_ammo_ < diff) {
        current_ammo_ += stored_ammo_;
        stored_ammo_ = 0;
    }
    else {
        current_ammo_ = max_ammo_;
        stored_ammo_ -= diff;
    }
}
bool Weapon::IsEmpty() {
    if (current_ammo_ > 0) return false;
    else return true;
}

void Weapon::SetStoredAmmunition(int amount) {
    stored_ammo_ = amount;
}

void Weapon::SetCurrentAmmunition(int amount) {
    current_ammo_ = amount;
}


