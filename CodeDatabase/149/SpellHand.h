#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "spell.h"
#include "Position.h"
#include <vector>
#include <memory>

class GameEngine;

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maximumSize;

public:
    explicit SpellHand(int maxSpells);
    bool addSpell(std::unique_ptr<Spell> spell);
    bool useSpell(int index, const Position& targetPosition, GameEngine& game);
    int getSpellCount() const;
    int getMaxSize() const;
    std::string getSpellName(int index) const;
    int getSpellManaCost(int index) const;
    std::string getSpellInfo(int index) const;
};

#endif