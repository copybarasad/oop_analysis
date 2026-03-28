#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Position.h"
#include <string>

class GameObject {
protected:
    int health;
    int maxHealth;
    std::string name;

public:
    GameObject(int health, const std::string& name);
    virtual ~GameObject() = default;
    
    int getHealth() const;
    int getMaxHealth() const;
    std::string getName() const;
    
    void setHealth(int newHealth);
    void takeDamage(int damage);
    bool isAlive() const;
    
    
    virtual std::string getType() const = 0;
};

#endif
