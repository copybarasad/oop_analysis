#pragma once
#include "Spell.h"
#include <string>

class DirectDamageSpell : public Spell {
private:
    std::string name;
    std::string description;
    int damage;
    int manaCost;
    int range; 

public:
    DirectDamageSpell(const std::string& spellName, int spellDamage, int cost, int spellRange);
    
    bool canCast(const Location& casterPos, const Location& targetPos, const GameBoard& board) const override;
    void cast(const Location& casterPos, const Location& targetPos, GameBoard& board) override;
    
    std::string getName() const override { return name; }
    std::string getDescription() const override { return description; }
    int getManaCost() const override { return manaCost; }
    int getRange() const override { return range; }
};