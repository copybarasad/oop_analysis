#pragma once

#include <utility>
#include <string>

class Entity {
protected:
    int health;
    int damage;

public:
    Entity(int startHealth, int startDamage);
    virtual ~Entity() = default;

    int getHealth() const;
    int getDamage() const;

    virtual void takeDamage(int damage);
    bool isAlive() const;

};
