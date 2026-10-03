#ifndef PICKUP_HPP
#define PICKUP_HPP

#include <string>
#include <sstream>
#include <fstream>
#include "player.hpp"


/**
 * @brief Helper for fetching floating point numbers from files matching to stat_name. 
 * @param stream file stream from which to search
 * @param stat_name name of the stat to be fetched 
 * @param cutoff stop searching if this string is encountered
 * @param default_value optional. Returned if no other value is found.
 * @return value found in file as float, default_value otherwise.
 */
float FloatForStat(std::ifstream& stream, std::string stat_name, std::string cutoff, float default_value=10.0f);

/**
 * @brief Helper for fetching integer data mathcing to stat_name.
 *  Uses FloatForStat internally.
 * @param stream file stream from which to search
 * @param stat_name name of the stat to be fetched 
 * @param cutoff stop searching if this string is encountered
 * @param default_value optional. Returned if no other value is found.
 * @return value found in file as int, default_value otherwise.
 */
int IntForStat(std::ifstream& stream, std::string stat_name, std::string cutoff, int default_value=10);

/**
 * @brief Pickup class is for creating pickups for the level.
 * 
 */
class Pickup {
public:
    Pickup(std::string n);
    /** All pickups have an Effect function for affecting a player.
     * @param player The one who is affected.
    */
    virtual std::string Effect(Player& player) = 0;
    const std::string GetName() const;

    /**
     * @brief Static function for creating new pickups
     * @param name Name of pickup in legend. Used to match for pickup type.
     * @return Pointer to the created pickup on success, but nullptr on failure.
     */
    static std::shared_ptr<Pickup> CreatePickup(std::string name, int grid_x, int grid_z);

    // For GUI
    int init_x = 0;
    int init_z = 0;
private:
    std::string name_;
};

/** @brief Ammo is a pickup which increases the player's ammo count for a given weapon.
 * @param count how many ammos the pickup gives
*/
class Ammo: public Pickup {
    public:
        Ammo(std::string name, int count);
        virtual std::string Effect(Player& player);
    private:
        int count_;
};

/** @brief Health is a pickup that heals the player. */
class Health: public Pickup {
    public:
        Health(std::string name, int count);
        virtual std::string Effect(Player& player);
    private:
        int count_;
};

/** @brief Hunger is a pickup that heals the player. */
class Hunger: public Pickup {
    public:
        Hunger(std::string name, int count);
        virtual std::string Effect(Player& player);
    private:
        int count_;
};

/** @brief Power boosts the player's power by a multiplier for a given time.
 * @param time the amount of time that the power is multiplied
 * @param mult multiplier
 */
class Power: public Pickup {
    public:
        Power(std::string name, double time, float mult);
        virtual std::string Effect(Player& player);
    private:
        double time_;
        double mult_;
};

/** @brief Speed boosts the player's speed by a multiplier for a given time.
 * @param time the amount of time that speed is multiplied
 * @param mult multiplier
 */
class Speed: public Pickup {
    public:
        Speed(std::string name, double time, float mult);
        virtual std::string Effect(Player& player);
    private:
        double time_;
        double mult_;
};

/** @brief Shield boosts the player's defence by a multiplier for a given time.
 * @param time the amount of time the defence is boosted
 * @param mult multiplier
 */
class Shield: public Pickup {
    public:
        Shield(std::string name, double time, float mult);
        virtual std::string Effect(Player& player);
    private:
        double time_;
        double mult_;
};

/** @brief Weapon pickup gives the player a new weapon to use.
 * @param name the name of the pickup
 * @param weapon_name the name of the weapon
 */
class WeaponPickup: public Pickup {
    public:
        WeaponPickup(std::string name, std::string weapon_name);
        virtual std::string Effect(Player& player);
    private:
        Weapon weapon_;
};

#endif