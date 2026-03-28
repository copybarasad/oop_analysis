#include "SpellFactory.h"

std::unique_ptr<Spell> SpellFactory::createRandomSpell() {
    return createSpell(getRandomSpellType());
}

std::unique_ptr<Spell> SpellFactory::createSpell(SpellType type) {
    switch (type) {
    case SpellType::DirectDamage:
        return std::make_unique<DirectDamageSpell>("Fireball", 30, 5, 25);
    case SpellType::AreaDamage:
        return std::make_unique<AreaDamageSpell>("Firestorm", 50, 4, 20);
    case SpellType::Trap:
        return std::make_unique<TrapSpell>("Bear Trap", 20, 3, 15);
    default:
        return std::make_unique<DirectDamageSpell>("Fireball", 30, 5, 25);
    }
}

std::unique_ptr<Spell> SpellFactory::createSpell(const std::string& name) {
    if (name == "Fireball") {
        return std::make_unique<DirectDamageSpell>("Fireball", 30, 5, 25);
    }
    else if (name == "Firestorm") {
        return std::make_unique<AreaDamageSpell>("Firestorm", 50, 4, 20);
    }
    else if (name == "Bear Trap") {
        return std::make_unique<TrapSpell>("Bear Trap", 20, 3, 15);
    }
    return createRandomSpell();
}

SpellFactory::SpellType SpellFactory::getRandomSpellType() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 2);

    return static_cast<SpellType>(dist(gen));
}