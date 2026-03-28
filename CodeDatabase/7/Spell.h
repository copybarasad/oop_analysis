#pragma once
#include <string>
#include <vector>
#include <iostream>

class Field;
class Player;
class Enemy;

class Spell {
public:
    virtual ~Spell() = default;
    virtual void cast(Player& caster, std::vector<Enemy>& enemies, Field& field, int targetX, int targetY) = 0;
    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
    virtual int getRange() const = 0;

    virtual void save(std::ostream& os) const = 0;
    virtual void load(std::istream& is) = 0;
};