#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "SpellCard.h"
#include <string>

class DirectDamageSpell : public SpellCard {
private:
    std::string name;
    std::string description;
    int manaCost;
    int range;
    int damage;

public:
    DirectDamageSpell(const std::string& spellName = "Fire Bolt", int spellDamage = 15, int spellManaCost = 10, int spellRange = 3);
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    int getRange() const override;
    bool canCast(int casterX, int casterY, int targetX, int targetY, GameField* field) const override;
    bool cast(Player& caster, int targetX, int targetY, GameField* field) override; // Changed to bool
    SpellCard* clone() const override;
};

#endif