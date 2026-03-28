#ifndef BOOK_OF_SPELLS_H
#define BOOK_OF_SPELLS_H

#include "spell.h"
#include <vector>
#include <memory>
#include <string>

class BookOfSpells {
public:
    BookOfSpells();
    
    std::unique_ptr<Spell> getRandomSpell() const;
    void displayAllSpells() const;
    std::unique_ptr<Spell> createSpellByName(const std::string& name) const;

private:
    std::vector<std::unique_ptr<Spell>> spellPrototypes;
};

#endif