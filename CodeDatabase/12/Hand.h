#ifndef HAND_H
#define HAND_H

#include <vector>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Spell.h"
#include "Fireball.h"
#include "Heal.h"
#include "Lighting.h"
#include "Trap.h"

class Hand {
private:
    int maxSize;
    std::vector<std::unique_ptr<Spell>> spells;
    void addRandomSpell();

public:
    Hand(int maxsize = 5);
    void addSpell(std::unique_ptr<Spell> spell);
    void showSpells() const;
    void useSpell(int index, Player& player, std::vector<Enemy>& enemies, Field& field);
    void spellBounty(int turnCount);
    void removeSpell(int index);
    bool isFull() const;
    int getSize() const;

};

#endif