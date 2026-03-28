#include "SpellFactory.h"
#include <iostream>

SpellFactory::SpellFactory() : gen(rd()) {}

std::unique_ptr<SpellCard> SpellFactory::createRandomSpell() {
    std::uniform_int_distribution<> dis(0, 3);
    int choice = dis(gen);
    
    std::unique_ptr<SpellCard> spell = nullptr;
    switch (choice) {
        case 0: 
            spell = createFireball();
            std::cout << "Created Fireball spell\n";
            break;
        case 1: 
            spell = createPunishmentOfZeus();
            std::cout << "Created Lightning Bolt spell\n";
            break;
        case 2: 
            spell = createFrosty();
            std::cout << "Created Frosty spell\n";
            break;
        case 3:
            spell = createTrapSpell();
            std::cout << "Created Beast Trap spell\n";
            break;
        default: 
            spell = createFireball();
            std::cout << "Created default Fireball spell\n";
            break;
    }
    return spell;
}

std::unique_ptr<SpellCard> SpellFactory::createFireball() {
    auto directSpell = std::make_unique<DirectDamageSpell>("Fireball", 40, 3, 4);
    return std::make_unique<SpellCard>(std::move(directSpell));
}

std::unique_ptr<SpellCard> SpellFactory::createPunishmentOfZeus() {
    auto directSpell = std::make_unique<DirectDamageSpell>("Lightning Bolt", 30, 2, 5);
    return std::make_unique<SpellCard>(std::move(directSpell));
}

std::unique_ptr<SpellCard> SpellFactory::createFrosty() {
    auto areaSpell = std::make_unique<AreaSpell>("Frosty", 25, 4, 3);
    return std::make_unique<SpellCard>(std::move(areaSpell));
}

std::unique_ptr<SpellCard> SpellFactory::createTrapSpell() {
    auto trapSpell = std::make_unique<TrapSpell>("Beast Trap", 25, 3, 3);
    return std::make_unique<SpellCard>(std::move(trapSpell));
}