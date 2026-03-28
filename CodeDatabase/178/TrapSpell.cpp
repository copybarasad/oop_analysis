#include "TrapSpell.h"
#include "GameController.h"
#include <iostream>
#include <cmath>

TrapSpell::TrapSpell(int dmg) : damage(dmg) {}

bool TrapSpell::apply(GameController &controller, const Position &casterPosition, const Position &targetPos)
{
    if (!controller.getField().isValidPosition(targetPos)) {
        std::cout << "Trap: invalid position.\n"; return false;
    }
    controller.addTrap(Trap(targetPos, damage));
    std::cout << "Trap placed at ("<<targetPos.x<<","<<targetPos.y<<") with "<<damage<<" dmg.\n";
    return true;
}