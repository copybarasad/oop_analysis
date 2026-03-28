#pragma once
#include "spell.h"

class SacredGlowSpell : public Spell {
public:
    SacredGlowSpell();
    SpellCastResult calculateCast(const Position& casterPos, 
                                const Position& targetPos) const override;
    bool canCast(const Position& casterPos, const Position& targetPos) const override;
};