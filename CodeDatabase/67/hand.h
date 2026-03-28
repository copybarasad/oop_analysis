#ifndef HAND_H
#define HAND_H

#include <vector>
#include "spell.h"

class Hand {
private:
    std::vector<Spell*> spells;
    int max_size;
    
public:
    Hand(int maximum_size);
    ~Hand();
    
    bool addSpell(Spell* spell);
    Spell* getSpell(int index);
    bool removeSpell(int index);
    int getSpellCount() const;
    int getMaxSize() const;
    bool isFull() const;
    void printSpells() const;
    void clearSpells();
};

#endif
