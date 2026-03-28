#pragma once
#pragma once
#include "Human.h"
#include <string>

class GameArea;

class Combatant : public Human {
public:
    Combatant(double h, double d, Coords coords) : Human(h, d, coords) {}
    virtual void generate_move(GameArea& gamearea);
    virtual bool attack(Coords target_coords, GameArea& gamearea) = 0;
};