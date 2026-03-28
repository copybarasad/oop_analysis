#include "spell_factory.h"

std::unique_ptr<ISpell> SpellFactory::createRandomSpell() {
    int type = rand() % 2; // 0 — прямой урон, 1 — по области

    if (type == 0)
        return std::make_unique<DirectDamageSpell>(100, 100);
    else
        return std::make_unique<AreaDamageSpell>(100, 100);
}

std::unique_ptr<ISpell> SpellFactory::createDirectDamage(int damage, int radius) {
    return std::make_unique<DirectDamageSpell>(damage, radius);
}

std::unique_ptr<ISpell> SpellFactory::createAreaDamage(int damage, int radius) {
    return std::make_unique<AreaDamageSpell>(damage, radius);
}