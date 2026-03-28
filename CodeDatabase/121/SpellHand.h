#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "Spell.h"
#include <array>
#include <memory>
#include <cstddef>

class SpellHand {
private:
    static constexpr size_t MAX_HAND_SIZE = 3;
    std::array<std::shared_ptr<Spell>, MAX_HAND_SIZE> spells;
    size_t spellCount;

public:
    SpellHand();
    
    bool addSpell(std::shared_ptr<Spell> spell);
    bool castSpell(size_t index, const Position& casterPos, const Position& targetPos, GameField& field);
    void removeSpell(size_t index);
    
    size_t getSpellCount() const;
    size_t getMaxSize() const;
    bool hasSpace() const;
    
    const Spell* getSpell(size_t index) const;

private:
    void compactSpells();
};

#endif