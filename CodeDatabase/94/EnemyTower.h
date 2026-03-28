#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "Position.h"

class EnemyTower {
private:
    Position position;
    int damage;
    int attackRange;
    int cooldown;
    int currentCooldown;

public:
    EnemyTower(const Position& pos);
    
    const Position& getPosition() const { return position; }
    int getDamage() const { return damage; }
    int getAttackRange() const { return attackRange; }
    int getCurrentCooldown() const { return currentCooldown; }
    
    // Добавляем метод для установки кулдауна
    void setCooldown(int cooldown) { currentCooldown = cooldown; }
    
    bool canAttack() const { return currentCooldown == 0; }
    void attack();
    void updateCooldown();
};

#endif