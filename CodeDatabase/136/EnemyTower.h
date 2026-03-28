#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "GameField.h"
#include "Wanderer.h"
#include <cmath>

class EnemyTower {
private:
    int attackRange;
    int damage;
    int attackCooldown;
    int currentCooldown;
    bool isActive;
    int health;
    int maxHealth;

public:
    EnemyTower(int range, int damage, int cooldown, int health = 40);
    
    void update();
    bool canAttack() const;
    bool isInRange(const Position& towerPos, const Position& targetPos) const;
    void performAttack(const Position& towerPos, const Position& playerPos, 
                      GameField& field, Wanderer& player);
    bool isDestroyed() const;
    void takeDamage(int damage);

    void setActive(bool active);
    bool getIsActive() const;
    int getAttackRange() const;
    int getHealth() const;
    int getMaxHealth() const;
};

#endif