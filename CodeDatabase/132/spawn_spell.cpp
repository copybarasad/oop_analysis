#include "spawn_spell.hpp"

SpawnSpell::SpawnSpell(char symbol): 
    Spell(symbol),
    lvl(0)
{};

int SpawnSpell::apply_enhancement(int current_lvl) {
    lvl = current_lvl;
    return 0;
}