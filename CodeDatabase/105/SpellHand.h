#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class SpellHand{
private:
    std::vector<std::unique_ptr<Spell>> spells;
    size_t maxSize;

public:
    SpellHand(size_t maxSize = 5);
    bool addSpell(std::unique_ptr<Spell> spell);
    bool removeSpell(size_t index);

    Spell* getSpell(size_t index) const;
    size_t getSpellCount() const;
    size_t getMaxSize() const;
    bool isFull() const;

    void clear();
};


#endif