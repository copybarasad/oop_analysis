#pragma once
#include <iostream>

#include "BasicSpell.h"

class TrapSpell : public BasicSpell {
public:
    TrapSpell(Type type, int damage, float range, std::string name) : BasicSpell(type, damage, range, name) {}

    void setTrap(Coordinates pos){ coordinates = pos;
}

    Coordinates getPosition() const { return coordinates; }

    json toJson() const override;
    void fromJson(const json& j) override;

private:
    Coordinates coordinates;
};