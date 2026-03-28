#ifndef ALLY_H
#define ALLY_H

#include "Position.h"

class Ally {
private:
    Position pos;
    int health;
    int damage;
    bool alive;

public:
    Ally(int x, int y);
    
    void makeMove(const Position& playerPos, int width, int height);
    void takeDamage(int damage);
    void setPos(int x, int y);
    
    const Position& getPos() const;
    bool isAlive() const;
    int getHealth() const;
    int getDamage() const;
};

#endif