#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "spell.h"
#include <deque>
#include <memory> 
#include <iostream>

class PlayerHand
{
private:
    std::deque<std::unique_ptr<ISpell>> spells;
    size_t maxSize;

public:
    explicit PlayerHand(size_t maxSize = 3) : maxSize(maxSize)
    {
        // spells.resize(maxSize);
    }

    bool addSpell(std::unique_ptr<ISpell> newSpell);
    void useSpell(size_t index, int targetX, int targetY, Entity* gamer);
    void display() const;
    const std::deque<std::unique_ptr<ISpell>> &getSpells() const { return spells; }
    void clearSpells() { spells.clear(); }
    void increaseMaxSize();
    void upgradeSpell(size_t index);
    void setMaxSize(int size) { maxSize = size; }
    int getMaxSize();
    int getSpellCount();
    void removeSpell();
};

#endif