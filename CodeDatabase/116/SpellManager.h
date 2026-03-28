#pragma once
#include "../eneties/Player.h"
#include "../spells/ISpell.h"
#include "../spells/DirectFightSpell.h"
#include "../spells/AreaDamageSpell.h"
#include "../spells/TrapSpell.h"
#include "../spells/SummonSpell.h"
#include "../spells/EnhancingSpell.h"
#include <random>
#include <vector>

class SpellManager {
private:
    std::random_device rd;
    std::mt19937 gen;

public:
    SpellManager();

    void upgradeAllSpells(Player& player);
    void removeHalfSpells(Player& player);
    void addRandomSpell(Player& player);
    void addSpecificSpell(Player& player, int spellType, int level = 1);
};