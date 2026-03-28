#ifndef GAME_ENEMY_TOWER_H
#define GAME_ENEMY_TOWER_H

#include "../utils/Point.h"
#include <iostream>

namespace Game {

class Game; 

class EnemyTower {
public:
    EnemyTower(Utils::Point position, int health = 100, int damage = 10, int range = 2, int cooldown = 4);
    EnemyTower(std::istream& is); // Конструктор для загрузки

    const Utils::Point& getPosition() const;
    int getHealth() const;
    int getDamage() const;
    int getAttackRange() const;
    bool isAlive() const;
    
    void takeDamage(int amount);
    
    bool canAttack() const;
    void attack(Game& game);
    void tickCooldown();

    void serialize(std::ostream& os) const;

private:
    void deserialize(std::istream& is);

    Utils::Point position;
    int health;
    int damage;
    int attackRange;
    int attackCooldown;
    int currentCooldown;
};

}

#endif