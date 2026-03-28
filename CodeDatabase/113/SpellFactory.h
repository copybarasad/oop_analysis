#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H

#include "Spell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <memory>
#include <random>

class SpellFactory {
public:
    enum class SpellType {
        DirectDamage,
        AreaDamage,
        Trap
    };

    static std::unique_ptr<Spell> createRandomSpell();
    static std::unique_ptr<Spell> createSpell(SpellType type);
    static std::unique_ptr<Spell> createSpell(const std::string& name);

private:
    static SpellType getRandomSpellType();
};

#endif // SPELLFACTORY_H