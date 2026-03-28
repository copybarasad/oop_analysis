#ifndef ENTITY_H
#define ENTITY_H

#include "Types.h"
#include <iostream>

class Entity {
protected:
    int health;
    int damage;
    Coords pos;

public:
    Entity() : health(0), damage(0), pos{ 0, 0 } {}
    Entity(int health, int damage, int x, int y);
    virtual ~Entity() = default;

    int getHealth() const;
    virtual int getDamage() const;
    Coords getCoords() const;

    void move(int dx, int dy);

    void takeDamage(int amount);
    bool isAlive() const;

    virtual void log() const = 0;
};

#endif // ENTITY_H

