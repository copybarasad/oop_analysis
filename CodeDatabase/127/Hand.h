#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>
#include <string>
#include "Spell.h"

class Player;
class Game;

class Hand {
public:
    explicit Hand(int capacity = 5);
    ~Hand() = default;

    Hand(const Hand&) = delete;
    Hand& operator=(const Hand&) = delete;

    Hand(Hand&&) noexcept = default;
    Hand& operator=(Hand&&) noexcept = default;

    int size() const;
    int getCapacity() const { return capacity_; }
    bool addSpell(std::unique_ptr<Spell> s);
    bool drawRandomSpell();
    bool buySpell(std::unique_ptr<Spell> s, Player& player);
    bool useSpell(int index, Game& game, Player& player, int tx, int ty, std::string* errorMessage = nullptr);

    void printSpellsWithDescription() const;
    const std::vector<std::unique_ptr<Spell>>& getSpells() const;

    // Новые методы для удаления карт при переходе уровня
    void removeSpellAt(int index);
    void removeRandomHalf();

private:
    int capacity_;
    std::vector<std::unique_ptr<Spell>> spells_;
};

#endif
