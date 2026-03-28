#pragma once
#include "Spell.h"
#include <vector>
#include <memory>

class Spell;

class Hand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;
    std::unique_ptr<Spell> createRandomSpell() const;

public:
    Hand(int maxSize = 5);
    
    Hand(const Hand& other);
    Hand& operator=(const Hand& other);
    Hand(Hand&& other) noexcept = default;
    Hand& operator=(Hand&& other) noexcept = default;
    
    ~Hand() = default;
    
    bool addSpell(std::unique_ptr<Spell> spell);
    
    bool removeSpell(int index);
    
    Spell* getSpell(int index) const;
    
    int getSpellCount() const;
    
    int getMaxSize() const;
    
    bool isFull() const;
    
    bool isEmpty() const;
};

