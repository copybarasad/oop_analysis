#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "spell.h"
#include <vector>
#include <memory>

class PlayerHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    size_t maxSize;
    
public:
    explicit PlayerHand(size_t maxSize);
    bool addSpell(std::unique_ptr<Spell> spell);
    size_t getSpellCount() const;
    size_t getMaxSize() const;
    std::unique_ptr<Spell> drawRandomSpell();
    const std::vector<std::unique_ptr<Spell>>& getSpells() const;
    bool isFull() const;
    void discardRandomHalf();
};

#endif