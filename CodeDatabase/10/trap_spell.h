#pragma once
#include "spell_basis.h"

class TrapSpell : public SpellBasis {
public:
    TrapSpell();
    
    bool canCast(int casterX, int casterY, int targetX, int targetY, const Field& field) const;
    int cast_spell(int targetX, int targetY, int , int , Field& field, Player& player) override;
};
