#pragma once

#include <string>
#include <iostream>

enum class SpellType 
{
    DEATH_NOTE,
    PSI_STORM, 
    TRAP,
    DOPPELGANGER,
    LEVEL_UP
};

class SpellHand 
{
protected:
    SpellType type;
    bool active;
    std::string name;
    std::string description;
    std::string upgrade_desc;
    int price;
    int damage;
    int range;
    int AoE;
    bool enemies_required;
    int cooldown;
    int LvL;
    int max_amount;

public:
    SpellHand(SpellType type, bool active = true);
    void Upgrade();
    SpellType getType() const;
    std::string getName() const;
    std::string getDescription() const;
    int getPrice() const;
    int getLvL() const;
    int getRange() const;
    int getAoE() const;
    bool requiresEnemy() const;
    int getDamage() const;
    int getMaxAmount() const;
};