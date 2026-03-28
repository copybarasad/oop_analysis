#include "SingleTargetSpell.h"

SingleTargetSpell::SingleTargetSpell(const std::string& name, int damage, int range)
    : Spell(name, "Deals " + std::to_string(damage) + " damage to single target", 
            range, SpellType::SINGLE_TARGET), damage(damage) {}