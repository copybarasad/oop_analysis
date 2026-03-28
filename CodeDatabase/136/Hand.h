#ifndef HAND_H
#define HAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class Hand {
private:
    std::vector<std::unique_ptr<SpellCard>> spells;
    int maxSize;
    int currentSize;

public:
    Hand(int maxSize = 3);
    ~Hand() = default;
    
    Hand(const Hand&) = delete;
    Hand& operator=(const Hand&) = delete;
    
    Hand(Hand&&) = default;
    Hand& operator=(Hand&&) = default;
    
    bool addSpell(std::unique_ptr<SpellCard> spell);
    bool removeSpell(int index);
    bool castSpell(int index, Position target, GameField& field);
    void resetSpells();
    
    int getSpellCount() const;
    int getMaxSize() const;
    SpellCard* getSpell(int index);
    const SpellCard* getSpell(int index) const;
    
    void displayHand() const;
};

#endif