#ifndef ENEMY_H
#define ENEMY_H

#include "game_field.h"
#include <vector>
#include "enemy_building.h"
#include "enemy_tower.h"

class Enemy {
private:
    static const int START_HEALTH = 3;
    static const int START_DAMAGE = 2;

    int health;
    int damage;
    int positionX;
    int positionY;

public:
    Enemy(int startX, int startY);
    
    int getHealth() const;
    void setHealth(int h);
    void setDamage(int d);
    int getDamage() const;
    int getPositionX() const;
    int getPositionY() const;

    void takeDamage(int damageAmount);
    bool isAlive() const;

    bool moveTowardsPlayer(int playerX, int playerY, const GameField& field, const std::vector<Enemy>& allEnemies, const EnemyBuilding& building, const EnemyTower& tower);
    
    void setPosition(int x, int y);
};

#endif