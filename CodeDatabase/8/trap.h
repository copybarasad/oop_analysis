#pragma once
#include "basespell.h"

class TrapSpell : public BaseSpell {
public:
    TrapSpell(int dmg, int cost, int radius, std::string spellName);
    bool do_spell(int x, int y, Field& field, Player& player, Enemy& enemy, std::string& message) override;
};