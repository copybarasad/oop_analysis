#pragma once

#include "Position.h"

class EnemyTower {
private:
    Position position;
    int health;
    int damage;
    int attackRange;
    int cooldown;
    int maxCooldown;

public:
    EnemyTower(int x, int y, int health = 40, int damage = 10, int attackRange = 3, int maxCooldown = 2);

    void take_Damage(int dmg);
    bool isAlive() const;
    int get_Health() const;
    int get_Damage() const;
    int get_AttackRange() const;
    const Position& get_Position() const;

    bool canAttack() const;

    bool isInRange(const Position& target) const;

    void performAttack();

    void updateCooldown();
    
    int get_Cooldown() const;
    int get_MaxCooldown() const;
    void setHealth(int h);
    void setCooldown(int cd);
};







