#pragma once
#include "Combatant.h"

class Summon : public Combatant {
public:
    Summon(double h, double d, Coords coords) : Combatant(h, d, coords) {}
    bool attack(Coords target_coords, GameArea& gamearea) override;
    bool operator!=(Coords& other) {
        return coords.x != other.x || coords.y != other.y;
    }
};