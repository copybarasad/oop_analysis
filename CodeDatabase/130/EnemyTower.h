#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "Position.h"

class EnemyTower {
private:
    Pos position_;
    int attackRange_;
    int attackDamage_;
    int attackCooldown_;
    int cooldownCounter_;
    int health_;
    int maxHealth_;

public:
    EnemyTower(Pos pos, int range = 4, int damage = 15, int cooldown = 3, int health = 80);

    Pos getPosition() const { return position_; }

    int getAttackRange() const { return attackRange_; }
    int getAttackDamage() const { return attackDamage_; }

    int getHealth() const { return health_; }
    int getMaxHealth() const { return maxHealth_; }
    bool isAlive() const { return health_ > 0; }
    void takeDamage(int amount);

    bool canAttack() const { return cooldownCounter_ >= attackCooldown_; }
    void nextTurn() { ++cooldownCounter_; }
    void resetCooldown() { cooldownCounter_ = 0; }

    bool isInRange(const Pos& target) const;
};

#endif

