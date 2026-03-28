#ifndef HAND_H
#define HAND_H

#include "../Enums.h"
#include "../Constants.h"
#include <random>
#include <vector>
#include <ctime>
#include <iostream>

class Hand{
private:
    std::vector<SpellType> spells;
    int size;
    int count;
    int upgradePower;
public:
    Hand(int size);
    Hand();
    ~Hand();

    void upgradeNextSpell(int power);
    void setUpgradePower(int upgradePower);
    int getUpgradePower();
    void setSize(int size);
    int getSize();
    SpellType getSpell(int index);
    int getCount();
    
    bool addSpell(SpellType spell);
    bool addRandomSpell();
    void removeSpell(int index);
    void clear();
    
    void displayHand();
};

#endif