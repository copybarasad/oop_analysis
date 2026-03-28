#pragma once
#include "GameObjects/Field.hpp"
#include "GameObjects/Entities/interfaces.hpp"
#include "types.hpp"
#include <vector>

class GameClass;

struct SpellContract {
    Coordinates coord;
    int damage;
    bool longLive;
};

class Spell {
public:
    virtual std::vector<SpellContract> use(Coordinates target) = 0;
    virtual std::vector<Coordinates> getCastRangeCoordinates(Coordinates castCoord, int mapH, int mapW) = 0;
    virtual ~Spell() = default;
    virtual bool canUseHere(FieldType ft) = 0;
    virtual SpellType getSpellType() = 0;
};

