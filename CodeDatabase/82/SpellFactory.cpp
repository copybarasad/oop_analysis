#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <random>

std::unique_ptr<Spell> SpellFactory::createRandomSpell() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    switch (dis(gen)) {
        case 0: return createFireball();
        case 1: return createFrostball();
        case 2: return createLightingball();
        case 3: return createExplosion();
        default: return createFireball();
    }
}

std::unique_ptr<Spell> SpellFactory::createFireball() {
    return std::make_unique<DirectDamageSpell>("Огненный шар", 35, 15, 4);
}

std::unique_ptr<Spell> SpellFactory::createFrostball() {
    return std::make_unique<DirectDamageSpell>("Ледяной шар", 25, 10, 5);
}

std::unique_ptr<Spell> SpellFactory::createLightingball() {
    return std::make_unique<DirectDamageSpell>("Электрический шар", 40, 20, 3);
}

std::unique_ptr<Spell> SpellFactory::createExplosion() {
    return std::make_unique<AreaDamageSpell>("Взрыв урон по области 2x2", 20, 25, 3, 2);
}