#pragma once
#include "GameObjects/Spells/Spell.hpp"
#include <GameClass/GameClass.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include "types.hpp"


class Trap : public Spell {
public:
    Trap(int lvl);

    std::vector<Coordinates> getCastRangeCoordinates(Coordinates castCoord, int mapH, int mapW) override;
    std::vector<SpellContract> use(Coordinates target) override;
    bool canUseHere(FieldType ft) override;
    SpellType getSpellType() override;

private:
    int range;
    int damage;
};
