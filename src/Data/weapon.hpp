#ifndef WEAPON_H
#define WEAPON_H
#include <string>

/**
 * @brief The weapon data class contains all the useful information for a given weapon.
 * @param name The name of the weapon
 * @param currentAmmo The current ammunition
 * @param maxAmmo The maximum ammunition
 * @param damage The damage dealt by the weapon
*/
class Weapon {
public:
    Weapon() = default;
    Weapon(std::string name, int currentAmmo, int maxAmmo, int stored_ammo, int damage, float attack_speed);
    Weapon(std::string name);
    /** @brief Returns current ammunition. If the weapon doesn't take ammunition it return -1.
     * @return amount of ammunition */
    int GetCurrentAmmo();
    /** @return amount of max ammunition. */
    int GetMaxAmmo();
    /** @return amount of stored ammunition. */
    int GetStoredAmmo();
    /** @return amount of damage dealt by the weapon. */
    int GetDamage();
    /** @return fire rate of weapon as min time between attacks in seconds. */
    float GetAttackSpeed() const;
    /** @return reload speed of weapon. */
    float GetReloadSpeed() const;
    /** @return range of weapon. */
    float GetRange() const;

    /** @return name of the weapon. */
    std::string GetName();

    /** @brief Increases stored ammunition. */
    void IncreaseStored(int amount);
    /** @brief Decreases stored ammunition. */
    void DecreaseCurrentAmmo(int amount);
    /** @brief Reloads the current ammunition back to the maximum if weapon takes ammunition. */
    void Reload();
    /** @return true if the player runs out of ammunition. */
    bool IsEmpty();

    /** 
     * @brief sets stored ammo 
     * @param amount amount of ammo as int
    */
    void SetStoredAmmunition(int amount);

    /**
     * @brief sets current ammo 
     * @param amount amount of ammo as int
    */
   void SetCurrentAmmunition(int amount);

private:
    //Variables for the current ammo, maximum ammo and the amount of damage.
    std::string name_;
    int current_ammo_, max_ammo_, stored_ammo_, damage_;
    float attack_speed_;
    float reload_speed_ = 1.0f;
    float range_ = 3.0f;
};

#endif