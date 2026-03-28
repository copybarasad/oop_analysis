#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "position.h"
#include <iostream>

class GameObject {
public:
    GameObject(const Position& position, int health, int damage);
    virtual ~GameObject() = default;

    int getHealth() const;
    int getDamage() const;
    const Position& getPosition() const;
    void setPosition(const Position& position);
    void takeDamage(int damage);
    bool isAlive() const;

    void save(std::ostream& out) const;
    void load(std::istream& in);

protected:
    int healthPoints;
    int damagePoints;
    Position currentPosition;

private:
    void validateHealth();
};

#endif



