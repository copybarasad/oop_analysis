#pragma once
#include <memory>
#include "Spell.h"

class SpellFactory {
public:
    static std::unique_ptr<Spell> createMagicMissile();
    static std::unique_ptr<Spell> createFireball();
    static std::unique_ptr<Spell> createTrap();

};
