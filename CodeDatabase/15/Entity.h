#ifndef ENTITY_H
#define ENTITY_H

#include "Point.h"
#include <iostream>

class Game;

class Entity {
public:
    Entity(int start_x, int start_y, int health, int damage);
    virtual ~Entity() = default;

    virtual void update(Game& game) {}

    Point getPosition() const;
    void setPosition(int new_x, int new_y);
    int getHealth() const;
    bool takeDamage(int amount);
    int getDamage() const;
    bool isAlive() const;
    
    virtual void save(std::ostream& os) const;
    virtual void load(std::istream& is);

protected:
    Point position_;
    int health_;
    int damage_;
};

#endif