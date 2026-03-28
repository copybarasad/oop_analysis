#include "SpellFactory.hpp"
#include <cstdlib>

#include "DirectDamageSpell.hpp"
#include "AreaOfEffectSpell.hpp"
#include "TrapSpell.hpp"
#include "SummonSpell.hpp"
#include "EnhancementSpell.hpp"


Spell* SpellFactory::createSpell(int typeId) {
    switch (typeId) {
        case 0: return new DirectDamageSpell();
        case 1: return new AreaOfEffectSpell();
        case 2: return new TrapSpell();
        case 3: return new SummonSpell();
        case 4: return new EnhancementSpell();
        default: return nullptr;
    }
}

Spell* SpellFactory::createRandomSpell() {
    int spell_type = rand() % 4;
    return createSpell(spell_type);
}