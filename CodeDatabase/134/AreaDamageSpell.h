#pragma once
#include "Spell.h"

class AreaDamageSpell : public Spell {
public:
    AreaDamageSpell(int damage, int range);
    void Use(Player* player, GameField* field, int targetX, int targetY) override;

private:
    int damage;
    int range;
};
