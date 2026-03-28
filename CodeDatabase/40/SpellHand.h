#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <vector>
#include <memory>
#include "Spell.h"

class SpellHand{
private:
    std::vector<std::unique_ptr<Spell>> spells;
    size_t maxSize;

public:
    SpellHand(size_t maxSpells);

    const Spell* getSpell(size_t index) const;
    size_t size() const;
    bool hasSpell(int spellId) const;
    bool addSpell(std::unique_ptr<Spell> spell);
    
    void save(std::ostream& out) const;
    void load(std::istream& in);
};

#endif