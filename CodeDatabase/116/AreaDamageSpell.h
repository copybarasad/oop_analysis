#pragma once
#include "IDamageSpell.h"
#include <iostream>
#include <cmath>

class DamageSpellContext;

class AreaDamageSpell : public IDamageSpell{
private:
    int maxR;
    int damage;
    int squareSide;
    int dSide;
    int lvl;
    bool spellWasUsed;
public:
    AreaDamageSpell(int lvl = 0, int maxR = 3, int damage = 13, int squareSide = 2, int dSide = 0):maxR(maxR), damage(damage), squareSide(squareSide), dSide(dSide), spellWasUsed(false), lvl(lvl){};
    ~AreaDamageSpell() override = default;
    std::string getName() override;
    void cast(ISpellContext& config) override;
    int getLVL() override { return lvl; };
    int getDamage() override { return damage; }
    void apply(DamageSpellContext& context) override;
    void apply(EnemySpellContext& context) override;
    void upgradeCard(int dlvl = 1) override{ lvl += dlvl;}
    void resetUpgrades() override{ lvl = 0; }
    bool wasUsed() override { return spellWasUsed; } 
};