#ifndef ENEMY_H
#define ENEMY_H

#include "GameTypes.h"
#include "Position.h"
#include <random>

class Enemy {
private:
    int health;
    int maxHealth;
    int damage;
    bool alive;
    bool slowed;
    bool skipNextTurn;
    Pos position;

public:
    Enemy(int hp = 50, int dmg = 15);

    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    bool isAlive() const;
    Pos getPosition() const;

    void takeDamage(int dmg);
    void die();
    void setPosition(int x, int y);
    void setPosition(const Pos& pos);

    Direction getRandomDirection();

    bool isSlowed() const;
    void setSlowed(bool value);
    bool shouldSkipTurn() const;
    void skipNextMove();
    void resetSkipTurn();
};

#endif