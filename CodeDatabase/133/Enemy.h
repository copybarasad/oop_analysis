#ifndef ENEMY_H
#define ENEMY_H
#include "GameConstants.h"

class Enemy {
private:
    int health;
    int damage;

public:
    Enemy(int enemyHealth = GameConstants::DEFAULT_ENEMY_HEALTH, int enemyDamage = GameConstants::DEFAULT_ENEMY_DAMAGE);

    int getHealth() const;
    int getDamage() const;

    void takeDamage(int Damage);
    bool isAlive() const;
};

#endif