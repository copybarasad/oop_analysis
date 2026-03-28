#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H

#include "spell_card.h"
#include <vector>
#include <memory>
#include <random>
#include <iostream>

class PlayerHand {
public:
    PlayerHand(int maxSize = 5);

    bool addSpell(std::unique_ptr<SpellCard> spell);
    bool removeSpell(int index);
    bool canAddSpell() const;
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;
    int getMaxSize() const;

    const std::vector<std::unique_ptr<SpellCard>>& getSpells() const;
    SpellCard* getSpell(int index) const;

    void clear();

    void save(std::ostream& out) const;
    void load(std::istream& in);

private:
    std::vector<std::unique_ptr<SpellCard>> spells;
    int maxHandSize;
};

#endif


