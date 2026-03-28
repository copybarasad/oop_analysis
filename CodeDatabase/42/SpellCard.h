#pragma once
#include <memory>
#include "Spell.h"

class SpellCard {
private:
    std::unique_ptr<Spell> spell;
    
public:
    SpellCard(std::unique_ptr<Spell> spellPtr);
    
    Spell* getSpell() const;
};
