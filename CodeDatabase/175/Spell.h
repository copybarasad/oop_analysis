#pragma once
#include <string>

class Field;
class Player;

class Spell {
public:
    virtual ~Spell() = default;
    virtual std::string getName() const = 0;
    virtual bool apply(Field& field, const Player& player, int targetX, int targetY) = 0;
    virtual bool needsTarget() const { return true; }
    virtual bool isBuff() const { return false; }
};