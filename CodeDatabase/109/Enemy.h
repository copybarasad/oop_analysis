#ifndef ENEMY_H
#define ENEMY_H

#include "Position.h"

class Enemy {
private:
    int health;
    int damage;
    Position pos;
    bool alive;

public:
    Enemy(int health = 30, int damage = 5);
    
    int getHealth() const;
    int getDamage() const;
    const Position& getPos() const;
    bool isAlive() const;
    
    void takeDamage(int damage);
    void move(int dx, int dy);
    void setPos(int x, int y);
    
    void checkAlive();
    void makeMove(const Position& playerPos, int width, int height);
};

#endif