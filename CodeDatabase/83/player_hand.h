#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "spell.h"
#include "book_of_spells.h"
#include <vector>
#include <memory>
#include <string>

class HandSpell {
public:
    HandSpell(BookOfSpells& book, int maxSize);
    
    void addSpell(std::unique_ptr<Spell> spell);
    bool removeSpell(int index);
    Spell* getSpell(int index) const;
    Spell* getSpellForName(const std::string& name) const;
    void displayAllName() const;
    
    int getCurrentSize() const { return spells.size(); }
    int getMaxSize() const { return maxSize; }
    void setMaxSize(int newMaxSize) { maxSize = newMaxSize; }

private:
    std::vector<std::unique_ptr<Spell>> spells;
    BookOfSpells& book;
    int maxSize;
};

#endif