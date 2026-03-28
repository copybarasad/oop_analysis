#include "SpellHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"

SpellHand::SpellHand() : spellCount(0) {
    // shared_ptr автоматически инициализируются nullptr
}

size_t SpellHand::getSpellCount() const {
    return spellCount;
}

size_t SpellHand::getMaxSize() const {
    return MAX_HAND_SIZE;
}

bool SpellHand::hasSpace() const {
    return spellCount < MAX_HAND_SIZE;
}

bool SpellHand::addSpell(std::shared_ptr<Spell> spell) {
    if (spellCount >= MAX_HAND_SIZE || !spell) {
        return false;
    }
    
    // Ищем первую свободную позицию
    for (size_t i = 0; i < MAX_HAND_SIZE; ++i) {
        if (!spells[i]) {
            spells[i] = spell;
            ++spellCount;
            return true;
        }
    }
    
    return false;
}

bool SpellHand::castSpell(size_t index, const Position& casterPos, const Position& targetPos, GameField& field) {
    if (index >= MAX_HAND_SIZE || !spells[index]) {
        return false;
    }
    
    auto& spell = spells[index];
    if (!spell->canCast(casterPos, targetPos, field)) {
        return false;
    }
    
    spell->cast(casterPos, targetPos, field);
    spells[index].reset();
    --spellCount;
    
    return true;
}

void SpellHand::removeSpell(size_t index) {
    if (index < MAX_HAND_SIZE && spells[index]) {
        spells[index].reset();
        --spellCount;
        compactSpells();
    }
}

const Spell* SpellHand::getSpell(size_t index) const {
    return (index < MAX_HAND_SIZE) ? spells[index].get() : nullptr;
}

void SpellHand::compactSpells() {
    size_t writeIndex = 0;
    for (size_t readIndex = 0; readIndex < MAX_HAND_SIZE; ++readIndex) {
        if (spells[readIndex]) {
            if (readIndex != writeIndex) {
                spells[writeIndex] = spells[readIndex];
                spells[readIndex].reset();
            }
            ++writeIndex;
        }
    }
}