#include "DirectSpell.h"
#include "Game.h"

bool DirectSpell::use(Game &game, std::pair<int, int> caster, std::pair<int, int> target) {
    if (!checkTarget(caster, target)) return false;
    if (caster != target) game.attack_cell(*this, target);
    return true;
}
