#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include <memory>
#include <cstdlib>
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"

class SpellFactory {
public:
    static std::unique_ptr<ISpell> createRandomSpell();

    static std::unique_ptr<ISpell> createDirectDamage(int damage, int radius);

    static std::unique_ptr<ISpell> createAreaDamage(int damage, int radius);
};

#endif // SPELL_FACTORY_H
