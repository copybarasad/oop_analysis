#ifndef INCLUDE_ENEMY_TOWER
#define INCLUDE_ENEMY_TOWER

#include "entity.h"

class EnemyTower : public Entity {
private:
    int spawn_interval_;
    int turns_until_spawn_;
    int damage_;
    int attack_range_;
    int attack_cooldown_;
    int current_attack_cooldown_;

public:
    EnemyTower();
    EnemyTower(int health, int spawn_interval, int damage = 15, int attack_range = 2, int attack_cooldown = 3);

    int spawn_interval() const;
    int turns_until_spawn() const;
    int damage() const;
    int attack_range() const;

    bool tick();
    bool tick_attack();
    void reset_spawn_timer();
};

#endif