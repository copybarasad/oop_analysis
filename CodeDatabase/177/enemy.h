#ifndef ENEMY_H
#define ENEMY_H

#include "position.h"

class Enemy {
private:
    int health;
    int damage;
    Position position;
    
    void validateHealth();

public:
    Enemy(int enemyHealth, int enemyDamage);

    void setHealth(int newHealth);
    void setDamage(int newDamage);
    
    int getHealth() const;
    int getDamage() const;
    Position getPosition() const;
    
    void setPosition(const Position& newPosition);
    void takeDamage(int damage);
    bool isAlive() const;
};

#endif