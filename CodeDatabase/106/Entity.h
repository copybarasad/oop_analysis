#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>

class Entity {
protected:
    std::string name;
    int x, y;
    int healthPoints;
    int maxHealthPoints;

public:
    Entity(const std::string& entityName, int startX, int startY, int health);
    virtual ~Entity() = default;

    virtual void move(int dx, int dy);
    virtual void setPos(int nx, int ny);
    virtual void takeDamage(int damage);
    virtual void heal(int amount);

    bool isAlive() const;

    const std::string& getName() const { return name; };
    std::pair<int, int> getPos() const { return { x, y }; };
    int getHp() const { return healthPoints; };
    int getMaxHp() const { return maxHealthPoints; };
};


#endif