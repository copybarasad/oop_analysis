#pragma once
#include "spell.hpp"

class EnhancementRules {
public:
    SpellParams Apply(SpellType type, SpellParams base, int stacks) const;
};
