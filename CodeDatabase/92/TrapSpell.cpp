#include "TrapSpell.h"
#include "Game.h"

bool TrapSpell::use(Game &game, std::pair<int, int> caster, std::pair<int, int> target) {
    if (!checkTarget(caster, target)) return false;
    return game.place_trap(target, attack());
}
