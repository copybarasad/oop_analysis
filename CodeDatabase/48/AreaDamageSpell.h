#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "SpellCard.h"
#include <string>

class AreaDamageSpell : public SpellCard {
private:
    std::string name;
    std::string description;
    int manaCost;
    int range;
    int damage;
    int areaSize;

public:
    AreaDamageSpell(const std::string& spellName = "Fireball", 
                   int spellDamage = 10, 
                   int spellManaCost = 15, 
                   int spellRange = 4,
                   int spellAreaSize = 2);
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    int getRange() const override;
    bool canCast(int casterX, int casterY, int targetX, int targetY, GameField* field) const override;
    bool cast(Player& caster, int targetX, int targetY, GameField* field) override; // Changed to bool
    SpellCard* clone() const override;
};

#endif