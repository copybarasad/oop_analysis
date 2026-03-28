#pragma once
#include "spell.h"

class HolyWaterSpell : public Spell {
public:
    HolyWaterSpell();
    SpellCastResult calculateCast(const Position& casterPos, 
                                const Position& targetPos) const override;
    bool canCast(const Position& casterPos, const Position& targetPos) const override;
};