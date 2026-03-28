#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "spell.h"
#include <vector>
#include <memory>

class SpellHand{
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;
    int currentMana;
    int maxMana;

public:
    SpellHand(int handSize);

    bool addSpell(std::unique_ptr<Spell> spell);
    bool castSpell(int index, Game& game, int targetX, int targetY);
    bool canCastSpell(int index) const;

    const std::vector<std::unique_ptr<Spell>>& getSpells() const;
    int getSpellCount() const;
    int getMaxSize() const;
    bool isFull() const;

    int getCurrentMana() const;
    int getMaxMana() const;
    void restoreMana(int amount);
    void setCurrentMana(int mana);
    void consumeMana(int amount);
    void increaseMaxMana(int amount);
    void addRandomSpell();
    void clearSpells();
    void enhanceAllSpells();
    void removeRandomSpells(int count);
};

#endif