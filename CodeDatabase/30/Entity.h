#pragma once
#include <iostream>

#include "../../gamecore/include/Enums.h"

// Базовая сущность: хранит HP и тип.
class Entity {
public:
    virtual ~Entity() = default;

    virtual void takeDamage(int dmg);
    virtual bool isAlive() const;

    virtual EntityType type() const = 0;

    virtual void save(std::ostream& os) const;
    virtual void load(std::istream& is);

protected:
    int hp_{0};
};
