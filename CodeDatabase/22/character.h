#pragma once
#include "field/position.h"



class Character
{
private:
    int health_;
    int damage_;
    Position position_;
    bool isSlow;

public:
    Character(int health, int damage, Position position);
    virtual ~Character();

    void takeDamage(int damage);
    bool isAlive();
    bool thisIsSlow();

    int getDamage() const;
    int getHealth() const;
    Position getPosition();

    void move(Position newPosition);
    void setHealth(int health);
    void setDamage(int damage);
    void setIsSlow(bool slow);
};
