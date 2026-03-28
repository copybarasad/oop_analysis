#pragma once
#include "Combatant.h"

class Enemy : public Combatant {
public:
    Enemy(double h, double d, Coords coords) : Combatant(h, d, coords) {}
    bool attack(Coords target_coords, GameArea& gamearea) override;
};