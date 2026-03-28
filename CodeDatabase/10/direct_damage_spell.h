#pragma once
#include "spell_basis.h"

class DirectDamageSpell : public SpellBasis {
public:
    DirectDamageSpell();
    
    bool canCast(int casterX, int casterY, int targetX, int targetY, int enemyX, int enemyY, Field& field) const;
    int cast_spell(int targetX, int targetY, int enemyX, int enemyY, Field& field, Player& player);
};
