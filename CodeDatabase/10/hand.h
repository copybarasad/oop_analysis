#pragma once
#include "spell.h"
#include <vector>

class Field;
class Player;
class Enemy;

class Hand {
private:
    std::vector<Spell*> cards;
    int maxSize;

public:
    Hand(int maxSize);
    ~Hand();
    
    bool addSpell(Spell* spell);
    int useSpell(int idx, int targetX, int targetY, int enemyX, int enemyY, Field& field, Player& player);
    int getSpellCount() const;
    bool isFull() const;
    bool isEmpty() const;
    int getMaxSize() const;
    void showHand() const;
    Spell* getSpell(int index) const;
    bool removeSpell(int index);
    const std::vector<Spell*>& getAllSpells() const;
    bool buySpell(int& playerPoints);
};
