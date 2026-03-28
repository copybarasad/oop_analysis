#include "damage_spell.hpp"

DamageSpell::DamageSpell(int range, int area_r, std::set<Effect> effects, char symbol):
    range(range), area_r(range), effects(effects), Spell(symbol)
{};