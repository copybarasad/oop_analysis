#pragma once

#include <iostream>
#include "Spell.h"

class ShopItem {
private:
    Spell* spell;
    std::string description;
    std::string title;
    int cost;
public:
    ShopItem(Spell* spell, std::string description, std::string title, int cost);

    ~ShopItem();
    std::string getDescription() const { return description; }
    std::string getTitle() const { return title; }
    int getCost() const { return cost; }

    Spell* getSpell() const { return spell; }
};