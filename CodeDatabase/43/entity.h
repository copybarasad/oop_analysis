#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
protected:
    int health;
    int maxHealth;
    int damage;
    std::string type;

public:
    Entity(int hp, int dmg, const std::string& entityType);
    virtual ~Entity() = default;

    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    const std::string& getType() const;
    bool isAlive() const;

    void takeDamage(int amount);
};

#endif