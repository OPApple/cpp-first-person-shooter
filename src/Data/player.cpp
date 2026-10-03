
#include "player.hpp"
#include "data_constants.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <cmath>

Player::Player(): hp_(basicHP), maxhp_(maxHP), hunger_(basicHunger), maxhunger_(maxHunger), move_speed_(basic_move_speed), weapons_(std::vector<std::shared_ptr<Weapon>>{}), currentWeaponIndex_(0) {}

void Player::SetHp(int hp) {
    hp_ = hp;
}

void Player::SetHunger(int hunger) {
    hunger_ = hunger;
}

void Player::SetMoveSpeed(float move_speed) {
    move_speed_ = move_speed;
}

void Player::SetDamageMult(float damage) {
    damageMult = damage;
}


void Player::SetReducMult(float reduction) {
    reducMult = reduction;
}

void Player::AddWeapon(Weapon& weapon) {
    std::shared_ptr<Weapon> ptr = std::make_shared<Weapon>(weapon);
    weapons_.push_back(ptr);
}

bool Player::RemoveWeapon(int slot) {
    if (slot < 1) return false;
    if (slot > (int)weapons_.size()) return false;
    
    int index = slot - 1;
    weapons_.erase(weapons_.begin() + index);
    return true;
}

int Player::Damage() {
    return std::ceil(GetCurrentWeapon()->GetDamage() * damageMult);
}

int Player::GetHp() {return hp_;}

int Player::GetHunger() {return hunger_;}

std::string Player::GetAmmo() {
    std::stringstream s;
    s << GetCurrentWeapon()->GetCurrentAmmo() << "/"
    << GetCurrentWeapon()->GetMaxAmmo() << " + " << GetCurrentWeapon()->GetStoredAmmo();
    return s.str();
}

float Player::GetMoveSpeed() {return move_speed_;}


std::shared_ptr<Weapon> Player::GetCurrentWeapon() {
    if (!weapons_.empty()) {
        return weapons_[currentWeaponIndex_];
    } else {
        return std::make_shared<Weapon>("Fist", 99, 99, 0, 1, 1.0f);
    }
}

bool Player::Dead() {
    if (hp_ > 0) {return false;}
    else return true;
}

void Player::IncreaseAmmo(int moreAmmo) {
    GetCurrentWeapon()->IncreaseStored(moreAmmo);
}

void Player::ChangeWeapon(int number) {
    if (number > (int)weapons_.size()) {
        currentWeaponIndex_ = weapons_.size() - 1;
    } else {
        currentWeaponIndex_ = number - 1;
    }
}

void Player::Reload() {
    reload_timer_ = 0.0f;
    GetCurrentWeapon()->Reload();
}

bool Player::IsReloading() {
    return reload_timer_ <= GetCurrentWeapon()->GetReloadSpeed();
}

void Player::Shoot() {
    attack_timer_ = 0.0f;
    GetCurrentWeapon()->DecreaseCurrentAmmo(1);
}

bool Player::CanShoot() {
    auto weapon = GetCurrentWeapon();
    return weapon && !weapon->IsEmpty() && attack_timer_ >= weapon->GetAttackSpeed() && !this->IsReloading();
}

void Player::RemovePickup(std::string name) {
    if (name == "speed") {
        this->SetMoveSpeed(basic_move_speed);
    } else if (name == "power") {
        this->SetDamageMult(1.0f);
    } else if (name ==  "shield") {
        this->SetReducMult(1.0f);
    }
}

void Player::Tick(float delta) {
    attack_timer_ += delta;
    reload_timer_ += delta;

    for (auto it = pickup_timers.begin(); it != pickup_timers.end();) {
        (*it)->timer += delta;
        if ((*it)->timer > (*it)->duration) {
            std::cout << "ENDED: " << (*it)->name << std::endl;
            RemovePickup((*it)->name);
            it = pickup_timers.erase(it);
        } else {
            ++it;
        }
    }

}

int Player::Heal(int amount) {
    int temp = this->GetHp();
    this->hp_ = std::min(this->hp_ + amount, maxHP);
    return this->GetHp() - temp;
}

int Player::Feed(int amount) {
    int temp = this->GetHunger();
    this->hunger_ = (this->hunger_ + amount) % maxHunger;
    return this->GetHunger() - temp;
}

void Player::DecreaseHP(int amount) {
    int amountWithRec = std::ceil(amount * reducMult);
    if (hunger_ > 0){
        if (amountWithRec > hunger_) {
            amountWithRec -= hunger_;
            hunger_ = 0;
        } else {
            hunger_ -= amountWithRec;
            amountWithRec = 0;
        }
    }
    if (amountWithRec > hp_) {hp_ = 0;}
    else {hp_ -= amountWithRec;}
}

void Player::DecreaseHunger(int amount) {
    if (amount > hunger_) {hunger_ = 0;}
    else {hunger_ -= amount;}
}

void Player::NextWeapon() {
    if (weapons_.size() > 1) {
        currentWeaponIndex_ = (currentWeaponIndex_ + 1) % weapons_.size();
    }
}

std::vector<std::shared_ptr<Weapon>> Player::GetWeapons() {
    return weapons_;
}

void Player::AddTimer(PickupTimer timer) {
    pickup_timers.push_back(std::make_shared<PickupTimer>(timer));
}

std::vector<std::shared_ptr<PickupTimer>> Player::GetTimers() {
    return pickup_timers;
}