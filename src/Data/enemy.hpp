#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <string>
#include <sstream>
#include <fstream>

enum MoveType {Chase, Bull, Sentry};

/**
 * @brief Helps with loading enemy data from file.
 * Also contains the default values in case an attribute is missing in enemies.txt.
 */
struct EnemyData {
    int hp              = 10;
    float range         = 10.0f;
    float attack_speed  = 1.0f;
    int damage          = 1;
    float move_speed    = 10.0f;
    MoveType move_type  = Sentry;
};

/**
 * @brief Helper for loading data from file.
 */
EnemyData LoadEnemyData(std::string name);

/**
 * @brief The enemy data class contains all the useful information for a given foe.
 * @param hp_ How much beating the creature can still take.
 * @param range_ In what distance the target creature must be to attack (unit: float distances)
 * @param attack_speed_ How long the creature needs to wait to attack again (unit: float seconds)
 * @param move_speed_ The movement speed of the creature (unit: float distance moved in a tick)
 * @param move_type_ Describes the movement type for the creature.
*/
class Enemy {
    public:  
        Enemy(const std::string& name, int hp, float range, float attack_speed, int damage, float move_speed, MoveType move_type);
        
        Enemy(const std::string& name, int grid_x, int grid_y);
        /** @return Creature's name as string. */
        std::string GetName() const;
        /** @return Creatues's range as float. */
        float GetRange() const;
        /** @return Speed of the creature as float. */
        float GetSpeed() const;
        /** @return Movetype of the creature. */
        MoveType GetMoveType() const;
        /** @return Current hp of creature as integer. */
        int GetHp() const;
        /** @brief Returns True if creature is alive. */
        bool Alive() const;
        /** @brief Returns True if creature is dead. */
        bool Dead() const;
        /** @brief Returns True if attack_timer >= attack_speed. */
        bool CanAttack() const;
        /** @brief Returns the damage for the attack as int and resets timer. */
        int Attack();
        /** @brief Ticks up the attack_timer each frame when not on pause. */
        void Tick(float delta);
        /** @brief Take damage lowers hp by given amount. */
        void TakeDamage(int dmg);

        static const int bull_check_frame = 120;

        bool is_attacking;
        
        // For GUI init pos
        int init_x;
        int init_z;

    private:
        std::string name_;
        int hp_;
        float range_;
        float attack_speed_;
        int damage_;
        float move_speed_;
        MoveType move_type_;
        /** @brief Attack timer resets each time the creature attacks, and
            prevents it from attacking until it goes past attack_speed again. */
        float attack_timer_ = 0;
};

#endif