#pragma once
#include "character.h"

class Enemy : public Character {
private:
    int attackPower;

public:
    Enemy(std::string name, Field& field, int startX, int startY);
    ~Enemy() = default;
    bool canAttack(int playerX, int playerY);
    bool moveTowards(int playerX, int playerY);
    int getAttackPower();
    void setPosition(int newX, int newY);
    void setLives(int newLives);
    void setDamage(int newDamage);
    void upgradeDamage(int increment = constants::ENEMY_DAMAGE_INCREMENT);
};
