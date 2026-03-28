#ifndef HAND_H
#define HAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class Hand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;

public:
    Hand(int maxSpells = 2) : maxSize(maxSpells) {}
    
    bool addSpell(std::unique_ptr<Spell> spell);
    bool removeSpell(int index);
    Spell* getSpell(int index) const;
    int getSpellCount() const { return spells.size(); }
    int getMaxSize() const { return maxSize; }
    bool isFull() const { return spells.size() >= maxSize; }
    
    // Rule of Five
    Hand(const Hand& other);
    Hand(Hand&& other) noexcept;
    Hand& operator=(const Hand& other);
    Hand& operator=(Hand&& other) noexcept;
    ~Hand() = default;
};

#endif