#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "SpellCard.h"
#include <vector>

class SpellHand {
private:
    std::vector<SpellCard*> spells;
    int maxSize;
    int currentSize;

public:
    SpellHand(int handSize = 5);
    ~SpellHand();
    
    SpellHand(const SpellHand&) = delete;
    SpellHand& operator=(const SpellHand&) = delete;
    
    SpellHand(SpellHand&& other) noexcept;
    SpellHand& operator=(SpellHand&& other) noexcept;
    
    bool addSpell(SpellCard* spell);
    bool removeSpell(int index);
    SpellCard* getSpell(int index) const;
    int getSpellCount() const;
    int getMaxSize() const;
    void displayHand() const; 
    void clear();
};

#endif