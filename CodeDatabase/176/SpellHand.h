#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "Spell.h"
#include "SpellEnhancementManager.h"
#include <vector>
#include <memory>

class GameManager;

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    size_t maxSize;

public:
    explicit SpellHand(size_t maxSpells);
    bool addSpell(std::unique_ptr<Spell> spell);
    std::unique_ptr<Spell> removeSpell(size_t index);

    bool castSpell(size_t index, GameManager& gameManager, int targetX, int targetY, SpellEnhancementManager& enhancementManager);
    
    Spell* getSpell(size_t index) const;
    size_t getSpellCount() const;
    size_t getMaxSize() const;
    bool isFull() const;
    void clear();
};

#endif
