#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include "ISpell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"
#include <memory>

class SpellFactory {
public:
    static std::unique_ptr<ISpell> createDirectDamageSpell(int damage = 15, int range = 3);
    static std::unique_ptr<ISpell> createAreaDamageSpell(int damage = 10, int width = 2, int height = 2, int range = 4);
    static std::unique_ptr<ISpell> createTrapSpell(int damage = 8);
    static std::unique_ptr<ISpell> createSummonSpell(int count = 1);
    static std::unique_ptr<ISpell> createUpgradeSpell(int power = 1);
    static std::unique_ptr<ISpell> createRandomSpell();
};

#endif