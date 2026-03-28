#ifndef SPELL_SHOP_H
#define SPELL_SHOP_H

#include <vector>
#include <memory>
#include "Spell.h"

class SpellShop{
private:
    std::vector<std::unique_ptr<Spell>> allSpells;

public:
    SpellShop();
    const Spell* getSpell(size_t index) const;
    size_t size() const;
};

#endif