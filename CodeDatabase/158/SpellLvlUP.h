#pragma once

#include "Player.h"
#include "SpellHand.h"
#include <iostream>

class SpellLvlUP 
{
private:
    Player& player;
public:
    SpellLvlUP(Player& p) : player(p) {}
    void displayUpgrades();
    bool upgradeSpell(int choice);
};