#pragma once
#include "spell_basis.h"

class AreaDamageSpell : public SpellBasis {
public:
    AreaDamageSpell();
    
    bool canCast(int casterX, int casterY, int targetX, int targetY, int enemyX, int enemyY, const Field& field) const;
    int cast_spell(int targetX, int targetY, int enemyX, int enemyY, Field& field, Player& player);
};
