#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    size_t maxSize;
    int mana;

public:
    SpellHand(size_t maximumSize = 5, int initialMana = 0);
    
    // Удаляем копирование, разрешаем перемещение
    SpellHand(const SpellHand&) = delete;
    SpellHand& operator=(const SpellHand&) = delete;
    
    SpellHand(SpellHand&& other) noexcept
        : spells(std::move(other.spells)),
          maxSize(other.maxSize),
          mana(other.mana) {
    }
    
    SpellHand& operator=(SpellHand&& other) noexcept {
        if (this != &other) {
            spells = std::move(other.spells);
            maxSize = other.maxSize;
            mana = other.mana;
        }
        return *this;
    }
    
    bool addSpell(std::unique_ptr<Spell> spell);
    bool removeSpell(size_t index);
    bool castSpell(size_t index, const Location& casterPos, const Location& targetPos, GameBoard& board);
    
    size_t getSpellCount() const { return spells.size(); }
    size_t getMaxSize() const { return maxSize; }
    bool isFull() const { return spells.size() >= maxSize; }
    
    int getMana() const { return mana; }
    void setMana(int newMana) { mana = newMana; }
    void addMana(int amount) { mana += amount; }
    bool spendMana(int amount);
    
    const Spell* getSpell(size_t index) const;
    void displaySpells() const;
};

#endif // SPELL_HAND_H