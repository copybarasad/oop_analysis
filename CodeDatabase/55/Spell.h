#pragma once
#include "GameConfig.h"
#include "Position.h"
#include "Enums.h"
#include "Serializable.h"
#include <string>
#include <vector>
class Player;
class Sea;
class Enemy;
class Octopus;

class Spell{
public:
    virtual ~Spell() = default;
    virtual int getDamage() = 0;
    virtual int getCost() = 0;
    virtual int getRange() = 0;
    virtual SpellFlag getSpellType() = 0;

    virtual std::string getSpellTypeString() const = 0;

    virtual bool attack(Player& pl, Sea& sea, std::vector<Enemy>& enemies, std::vector<Octopus>& octopuses, const Position& target) = 0;

    virtual void saveTo(SaveFileRAII &out) const = 0;
    virtual void loadFrom(SaveFileRAII &in) = 0;
};