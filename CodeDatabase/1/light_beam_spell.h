#pragma once
#include "spell.h"

class LightBeamSpell : public Spell {
public:
    LightBeamSpell();
    SpellCastResult calculateCast(const Position& casterPos, 
                                const Position& targetPos) const override;
    bool canCast(const Position& casterPos, const Position& targetPos) const override;
};