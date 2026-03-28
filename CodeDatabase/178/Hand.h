#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>
#include "ISpell.h"

class Player;

class Hand
{
private:
    std::vector<std::unique_ptr<ISpell>> spells;
    int maxSize;

public:
    Hand(int maxSize = 5);
    void addSpell(std::unique_ptr<ISpell> spell);
    bool buySpell(std::unique_ptr<ISpell> spell, Player &player, int cost);
    void removeAt(int index);
    int size() const;
    const ISpell* getAt(int index) const;
    std::unique_ptr<ISpell> extractAt(int index);
    void list() const;
    void listSpells(const Player &player) const;
    bool upgradeSpell(int index, int upgradePower);
    const ISpell* getSpellInfo(int index) const;
};

#endif