#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include "DirectDamageSpell.h"
#include "AreaSpell.h"
#include "TrapSpell.h"
#include <random>
#include <memory>

class SpellFactory {
private:
    std::random_device rd;
    std::mt19937 gen;

public:
    SpellFactory();
    std::unique_ptr<SpellCard> createRandomSpell();
    std::unique_ptr<SpellCard> createFireball();
    std::unique_ptr<SpellCard> createPunishmentOfZeus();
    std::unique_ptr<SpellCard> createFrosty();
    std::unique_ptr<SpellCard> createTrapSpell(); 
};

#endif