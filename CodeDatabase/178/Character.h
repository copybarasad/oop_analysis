#ifndef CHARACTER_H
#define CHARACTER_H

#include "Position.h"

class Character
{
protected:
    Position position;
    int health;
    int maxHealth;
    int damage;

public:
    Character(const Position &startPosition, int initialHealth, int initialDamage);
    virtual ~Character() = default;
    Position getPosition() const;
    void setPosition(const Position &newPosition);
    int getHealth() const;
    void takeDamage(int damageAmount);
    bool isAlive() const;
    int getDamage() const;
    virtual void move(const Position &newPosition) = 0;
};

#endif