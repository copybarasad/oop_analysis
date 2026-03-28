#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "spell.h"
#include <vector>
#include <memory>

class SpellHand {
private:
    std::vector<std::shared_ptr<Spell>> spells;
    size_t maxSize;

public:
    SpellHand(size_t maxHandSize);
    
    size_t getSize() const;
    size_t getMaxSize() const;
    bool isFull() const;
    bool isEmpty() const;
    
    bool addSpell(std::shared_ptr<Spell> spell);
    bool removeSpell(int index);
    std::shared_ptr<Spell> getSpell(int index) const;
    const std::vector<std::shared_ptr<Spell>>& getAllSpells() const;
    
    void clear();
};

#endif