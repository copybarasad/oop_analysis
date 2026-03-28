#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "SpellCard.h"
#include <string>

class TrapSpell : public SpellCard {
private:
    std::string name;
    std::string description;
    int manaCost;
    int range;
    int damage;
    int duration;

public:
    TrapSpell(const std::string& spellName = "Bear Trap", 
             int spellDamage = 20, 
             int spellManaCost = 8, 
             int spellRange = 2,
             int spellDuration = 5);
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    int getRange() const override;
    bool canCast(int casterX, int casterY, int targetX, int targetY, GameField* field) const override;
    bool cast(Player& caster, int targetX, int targetY, GameField* field) override; // Changed to bool
    SpellCard* clone() const override;
    
    int getDamage() const;
    int getDuration() const;
};

#endif