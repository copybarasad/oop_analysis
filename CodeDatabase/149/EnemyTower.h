#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "Enemy.h"

class EnemyTower : public Enemy {
private:
    int attackRange;
    int attackCooldown;


public:
    EnemyTower(int initialHealth, int towerDamage, int range);
    bool isBuilding() const override;
    bool canAttack() const;
    void resetCooldown();
    int getAttackRange() const;
    void decreaseCooldown();
    int getCooldown() const;
    void setCooldown(int cooldown = 2);
};

#endif