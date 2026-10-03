#include "pickup.hpp"
#include <sstream>
#include <iostream>

Pickup::Pickup(std::string name) : name_(name) {};

Ammo::Ammo(std::string name, int count) : Pickup(name), count_(count) {};
Health::Health(std::string name, int count) : Pickup(name), count_(count) {};
Hunger::Hunger(std::string name, int count) : Pickup(name), count_(count) {};
Power::Power(std::string name, double count, float mult) : Pickup(name), time_(count), mult_(mult) {};
Speed::Speed(std::string name, double count, float mult) : Pickup(name), time_(count), mult_(mult) {};
Shield::Shield(std::string name, double count, float mult) : Pickup(name), time_(count), mult_(mult) {};

WeaponPickup::WeaponPickup(std::string name, std::string weapon_name) : Pickup(name) {
    weapon_ = Weapon(weapon_name);
}


const std::string Pickup::GetName() const {
    return name_;
};

float FloatForStat(std::ifstream& stream, std::string stat_name, std::string cutoff, float default_value){
    std::string current;
    std::streampos read_start = stream.tellg(); // save start position for rewinding

    float res = default_value;
    while (std::getline(stream, current)) {
        if (current == cutoff) break;
        if (current.find(stat_name) != std::string::npos) {
            try {
                res = std::stof(current.substr(stat_name.length() + 1, current.length()));
            } catch (std::invalid_argument const& ex) {
                std::cout << "Invalid argument for " << stat_name << ". Using defaults..." << std::endl;
            }
            break;
        }
    }
    stream.seekg(read_start); // reverse
    return res;
}

int IntForStat(std::ifstream& stream, std::string stat_name, std::string cutoff, int default_value) {
    return static_cast<int>(FloatForStat(stream, stat_name, cutoff, default_value));
}


std::shared_ptr<Pickup> Pickup::CreatePickup(std::string name, int grid_x, int grid_z) {
    std::shared_ptr<Pickup> new_pickup = nullptr;
    std::ifstream data_file("resources/data/pickups.txt");
    
    std::string current;
    std::string type = "ammo";

    while (std::getline(data_file, current)) {
        if (current.find(name) != std::string::npos) {
            type = current.substr(name.length() + 1, current.length());
            break;
        }
    }

    if (type == "ammo") {
        int amount = IntForStat(data_file, "amount", "end " + name);
        std::cout << name << " got: " << amount << std::endl;
        new_pickup = std::make_shared<Ammo>(name, amount);
    }
    else if (type == "health") {
        int amount = IntForStat(data_file, "amount", "end " + name);
        std::cout << name << " got: " << amount << std::endl;
        new_pickup = std::make_shared<Health>(name, amount);
    }
    else if (type == "hunger") {
        int amount = IntForStat(data_file, "amount", "end " + name);
        std::cout << name << " got: " << amount << std::endl;
        new_pickup = std::make_shared<Hunger>(name, amount);
    }
    else if (type == "power") {
        float time = FloatForStat(data_file, "time", "end " + name);
        float mult = FloatForStat(data_file, "multiplier", "end " + name);
        new_pickup = std::make_shared<Power>(name, time, mult);
    }
    else if (type == "speed") {
        float time = FloatForStat(data_file, "time", "end " + name);
        float amount = FloatForStat(data_file, "amount", "end " + name);
        std::cout << name << " got: " << time << ", " << amount << std::endl;
        new_pickup = std::make_shared<Speed>(name, time, amount);
    }
    else if (type == "shield") {
        float time = FloatForStat(data_file, "time", "end " + name);
        float mult = FloatForStat(data_file, "multiplier", "end " + name);
        new_pickup = std::make_shared<Shield>(name, time, mult);
    }
    else if (type == "weapon") {
        new_pickup = std::make_shared<WeaponPickup>(name, name);
    }

    if (new_pickup) {
        new_pickup->init_x = grid_x;
        new_pickup->init_z = grid_z;
    }

    return new_pickup;
}

std::string Ammo::Effect(Player& player) {
    std::stringstream res;
    player.IncreaseAmmo(count_);
    res << count_ << " got ammo";
    return res.str();
};

std::string Health::Effect(Player& player) {
    std::stringstream res;
    res << player.Heal(count_) << " hp healed";
    return res.str();
};

std::string Hunger::Effect(Player& player) {
    std::stringstream res;
    player.Feed(count_);
    res << GetName() << " " << count_ << " hunger";
    return res.str();
};

std::string Power::Effect(Player& player) {
    PickupTimer new_timer;
    new_timer.duration = time_;
    new_timer.name = "power";
    player.AddTimer(new_timer);
    player.SetDamageMult(this->mult_);
    
    std::stringstream s;
    s << "Player" << "'s attacks " << mult_ << "x for " << time_ << "s";
    return s.str();
};

std::string Speed::Effect(Player& player) {
    PickupTimer new_timer;
    new_timer.duration = time_;
    new_timer.name = "speed";
    player.AddTimer(new_timer);
    player.SetMoveSpeed(this->mult_);
    std::stringstream s;
    s << "player" << "'s speed " << mult_ << "x for " << time_ << "s";
    return s.str();
};

std::string Shield::Effect(Player& player) {
    PickupTimer new_timer;
    new_timer.duration = time_;
    new_timer.name = "shield";
    player.AddTimer(new_timer);
    player.SetReducMult(this->mult_);
    std::stringstream s;
    s << "player" << "'s defence " << mult_ << "x for " << time_ << "s";
    return s.str();
};

std::string WeaponPickup::Effect(Player& player) {
    player.AddWeapon(weapon_);
    std::stringstream s;
    s << "Picked up " << weapon_.GetName() << "!";
    return s.str();
}