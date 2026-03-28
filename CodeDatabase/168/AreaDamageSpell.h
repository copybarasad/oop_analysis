#pragma once

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int range;
    int areaSize;  

public:
    AreaDamageSpell(int damage = 15, int range = 4, int areaSize = 2);

    SpellResult cast(GameController& game, const Position& target, EnhancementContext& enhancement) override;
    std::string getName() const override;
    std::string getDescription() const override;
    char getSymbol() const override;
    int getRange() const override;
    std::unique_ptr<Spell> clone() const override;
    
    int getDamage() const { return damage; }
    int getAreaSize() const { return areaSize; }
    void setDamage(int dmg) { damage = dmg; }
    void setRange(int r) { range = r; }
    void setAreaSize(int size) { areaSize = size; }
};
