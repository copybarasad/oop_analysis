#pragma once
#include "Spell.h"

class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int damage, int range);
    void Use(Player* player, GameField* field, int targetX, int targetY) override;

private:
    int damage;
    int range;
};
