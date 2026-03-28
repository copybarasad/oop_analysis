#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <memory>
#include "../field/Position.h"

class GameObject {
public:
    virtual ~GameObject() = default;
    
    virtual bool isAlive() const = 0;
    virtual void takeDamage(int amount) = 0;
    virtual Position getPosition() const = 0;
    
    virtual std::string serialize() const = 0;
};

#endif