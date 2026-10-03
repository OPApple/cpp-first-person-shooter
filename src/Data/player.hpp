#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "weapon.hpp"
#include <vector>
#include <memory>

/**
 * @brief A struct for tracking and managing timed pickups.
 * 
 */
struct PickupTimer {
    std::string name;
    float duration;
    float timer = 0.0f;
};

/**
 * @brief The player data class contains information of a given player and methods to manage the information.
*/
class Player {
public:
    Player();
    /** @brief Set the player's hp. 
     * @param hp amount of health
    */
    void SetHp(int hp);
    /** @brief Set the player's hunger. 
     * @param hunget amount of hunger
    */
    void SetHunger(int hunger);
    /** @brief Set the player's movement speed. 
     * @param move_speed amount of movements speed
    */
    void SetMoveSpeed(float move_speed);
    /** @brief Set the player's damage multiplier. 
     * @param damage amount of damage
    */
    void SetDamageMult(float damage);
    /** @brief Set the player's reduction to taken damage.
     * @param reduction amount of reduction
    */
    void SetReducMult(float reduction);
    /** @brief Adds a weapon to player's weapons. 
     * @param weapon weapon to be added
    */
    void AddWeapon(Weapon& weapon); 
    /** @brief Returns how much damage player deals. 
    */
    int Damage();
    /** @brief Removes a weapon from player's weapons. The index of the weapon is n-1.
     * @param slot weapon slot index from 1 to 9.
    */
    bool RemoveWeapon(int slot);

    /** @return player's HP as an integer. */
    int GetHp();
    /** @return player's hunger as an integer. */
    int GetHunger();
    /** @return player's ammunition as a string. */
    std::string GetAmmo();
    /** @return player's movement speed as a float. */
    float GetMoveSpeed();

    /** @return List of pointers to player's weapons. */
    std::vector<std::shared_ptr<Weapon>> GetWeapons();

    /** @return player's current weapon. */
    std::shared_ptr<Weapon> GetCurrentWeapon();

    /** @brief Returns True if the player is dead. 
     * The player is dead if they have 0 HP left.*/
    bool Dead();

    /** @brief Increases the player's amount of ammo, if weapon takes ammunition.
     *  @param moreAmmo amount of ammo
     */
    void IncreaseAmmo(int moreAmmo);

    /** @brief Changes to a weapon that's in the weapons in index numnber-1.
     *  @param number Weapon slot 
    */
    void ChangeWeapon(int number);

    /** @brief Reloads the current weapon. */
    void Reload();

    /** @brief Checks if player is reloading. */
    bool IsReloading();

    /** @brief Player shoots. */
    void Shoot();

    /** @brief Check if player has ammo to shoot. */
    bool CanShoot();

    /** @brief Advances timers if game not on pause. This includes pickups. */
    void Tick(float delta);

    /** @brief Decreases the player's health by given amount.
     *  @param amount how much hp should be decreased */
    void DecreaseHP(int amount);

    /** @brief Heals player for amount. Does not go past max hp.
     * @param amount how much hp should be restored
     * @return amount of hp restored
     */
    int Heal(int amount);

    /** @brief Decreases player's hunger for amount. Does not go past max hunger.
     * @param amount how much hunger should be decreased
     */
    void DecreaseHunger(int amount);
    
     /** @brief Increases value of hunger. Does not go past max hunger.
     * @param amount how much hunger should be restored
     * @return amount of hunger restored
     */
    int Feed(int amount);

    /** @brief Changes the current weapon to the next in the list of weapons.
     * Does nothing if the player has only one waepon.
     */
    void NextWeapon();

    /**
     * @brief Adds a pickup timer to player.
     * @param pu_timer timer struct to be added
     * 
     */
    void AddTimer(PickupTimer pu_timer);

    std::vector<std::shared_ptr<PickupTimer>> GetTimers();


private:
    int hp_, maxhp_, hunger_, maxhunger_;
    float move_speed_;
    float attack_timer_ = 0;
    float reload_timer_ = 0;
    std::vector<std::shared_ptr<Weapon>> weapons_;
    int currentWeaponIndex_;
    float damageMult = 1; 
    float reducMult = 1;
    std::vector<std::shared_ptr<PickupTimer>> pickup_timers = {};

    void RemovePickup(std::string name);

};

#endif