#include "base_spell.h"

BaseSpell::BaseSpell(const std::string& spell_name, const std::string& desc, int spell_range, int spell_cost, bool needs_target)
    : name(spell_name), description(desc), range(spell_range), cost(spell_cost), needs_target(needs_target) {}  