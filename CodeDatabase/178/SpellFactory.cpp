#include "SpellFactory.h"
#include <cstdlib>
#include <ctime>

std::unique_ptr<ISpell> SpellFactory::createDirectDamageSpell(int damage, int range) {
    return std::make_unique<DirectDamageSpell>(damage, range);
}

std::unique_ptr<ISpell> SpellFactory::createAreaDamageSpell(int damage, int width, int height, int range) {
    return std::make_unique<AreaDamageSpell>(damage, width, height, range);
}

std::unique_ptr<ISpell> SpellFactory::createTrapSpell(int damage) {
    return std::make_unique<TrapSpell>(damage);
}

std::unique_ptr<ISpell> SpellFactory::createSummonSpell(int count) {
    return std::make_unique<SummonSpell>(count);
}

std::unique_ptr<ISpell> SpellFactory::createUpgradeSpell(int power) {
    return std::make_unique<UpgradeSpell>(power);
}

std::unique_ptr<ISpell> SpellFactory::createRandomSpell() {
    int r = rand() % 5;
    switch(r) {
        case 0: return createDirectDamageSpell();
        case 1: return createAreaDamageSpell();
        case 2: return createTrapSpell();
        case 3: return createSummonSpell();
        case 4: return createUpgradeSpell();
        default: return createDirectDamageSpell();
    }
}