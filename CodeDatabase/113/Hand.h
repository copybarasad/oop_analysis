#ifndef HAND_H
#define HAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class Player;
class GameField;

class Hand {
private:
    std::vector<std::unique_ptr<Spell>> m_spells;
    size_t m_maxSize;

public:
    Hand(size_t maxSize);

    Hand(const Hand&) = delete;
    Hand& operator=(const Hand&) = delete;

    Hand(Hand&& other) noexcept;
    Hand& operator=(Hand&& other) noexcept;

    bool addSpell(std::unique_ptr<Spell> spell);
    void removeSpell(size_t index);
    Spell* getSpell(size_t index) const;
    size_t getSpellCount() const;
    size_t getMaxSize() const;
    void displaySpells(const Player& player, const GameField& field) const;
};

#endif // HAND_H