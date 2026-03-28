#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "Position.h"
#include "Player.h"

class EnemyTower {
private:
    Position pos;
    int health;
    int attackCooldown; // 0 = может атаковать
    static const int ATTACK_RANGE = 2;
    static const int ATTACK_DAMAGE = 6;
    static const int COOLDOWN_TURNS = 2;
    static const int MAX_HEALTH = 50;

public:
    EnemyTower(int x, int y);
    
    void takeDamage(int damage);
    bool canAttack(const Position& playerPos) const;
    void attack(Player& player);
    void updateCooldown();
    
    const Position& getPos() const;
    int getHealth() const;
    bool isAlive() const;
    void setHealth(int newHealth) {health = newHealth; }
};

#endif