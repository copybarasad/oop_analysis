#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class Game;

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;

public:
    SpellHand(int maxSize = 3);
    
    bool addSpell(std::unique_ptr<Spell> spell);
    bool useSpell(int index, Game& game, int targetX, int targetY);
    void displaySpells() const;
    int getSpellCount() const;
    bool isFull() const;
    void removeSpell(int index);
    const std::vector<std::unique_ptr<Spell>>& getSpells() const;
};

#endif