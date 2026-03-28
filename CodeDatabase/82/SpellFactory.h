#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H
#include "Spell.h"
#include <memory>

class SpellFactory {
public:
    static std::unique_ptr<Spell> createRandomSpell();
    static std::unique_ptr<Spell> createFireball();
    static std::unique_ptr<Spell> createFrostball();
    static std::unique_ptr<Spell> createLightingball();
    static std::unique_ptr<Spell> createExplosion();
private:
    SpellFactory() = default;
};

#endif