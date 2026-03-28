#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "character.h"
#include <utility>

class EnemyTower : public Character {
public:
    EnemyTower(int row, int col, int health = 5, int attack_radius = 2, 
               int damage = 2, int cast_interval = 2);
    std::pair<int,int> Position() const;
    int DirectDamageSpell() const;
    bool ShouldCast(const std::pair<int,int>& player_pos);

    int GetAttackRadius() const { return attack_radius_; }
    int GetCastInterval() const { return cast_interval_; }
    int GetCounter() const { return counter_; }
    void SetCounter(int counter) { counter_ = counter; }

private:
    int row_, col_;
    int attack_radius_;
    int cast_interval_;
    int counter_;
};

#endif