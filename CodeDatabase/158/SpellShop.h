#pragma once

#include "Player.h"
#include "SpellHand.h"
#include <iostream>

class SpellShop 
{
private:
    Player& player;
public:
    SpellShop(Player& p) : player(p) {}
    void displayShop();
    bool buySpell(int choice);
};