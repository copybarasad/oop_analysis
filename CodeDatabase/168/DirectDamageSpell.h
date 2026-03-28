#pragma once

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;

public:
    DirectDamageSpell(int damage = 25, int range = 3);

    SpellResult cast(GameController& game, const Position& target, EnhancementContext& enhancement) override;
    std::string getName() const override;
    std::string getDescription() const override;
    char getSymbol() const override;
    int getRange() const override;
    std::unique_ptr<Spell> clone() const override;
    
    int getDamage() const { return damage; }
    void setDamage(int dmg) { damage = dmg; }
    void setRange(int r) { range = r; }
};
