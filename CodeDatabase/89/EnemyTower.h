#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include <utility>

class EnemyTower {
public:
    EnemyTower(int health, int damage, int x, int y, int attackRange = 2);

    bool isAlive() const;
    void takeDamage(int amount);
    int getHealth() const; 
    int getDamage() const;
    std::pair<int, int> getPosition() const;
    int getAttackRange() const;

    bool canAttackThisTurn() const;
    void markAttackUsed();
    void update();

private:
    int health;
    int damage;
    int attackRange;
    std::pair<int, int> position;
    int attackCooldown;
};

#endif