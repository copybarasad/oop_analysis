#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;

public:
    SpellHand(int size);
    bool addSpell(std::unique_ptr<Spell> spell);
    bool removeSpell(int index);
    void clearHand();
    bool castSpell(int index, const SpellTarget& target,
        GameField& field, std::vector<Enemy>& enemies, Player& player) const;
    std::string getSpellName(int index) const;
    std::string getSpellDescription(int index) const;
    int getSpellManaCost(int index) const;
    int getSpellCount() const;
    int getMaxSize() const;
    bool isFull() const;
    void displaySpells() const;
};

#endif
