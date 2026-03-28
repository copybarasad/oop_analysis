#pragma once
#include "IDamageSpell.h"
#include <iostream>
#include <cmath>

class DamageSpellContext;
class EnemySpellContext;

class DirectFightSpell : public IDamageSpell{
private:
    int maxR;
    int damage;
    int dR;
    int lvl;
    bool spellWasUsed;
public:
    DirectFightSpell(int lvl = 0, int maxR = 5, int damage = 18, int dR = 1):maxR(maxR), damage(damage), dR(dR), lvl(lvl), spellWasUsed(false){};
    ~DirectFightSpell() override = default;
    std::string getName() override;
    void cast(ISpellContext& context) override;
    void apply(DamageSpellContext& context) override;
    void apply(EnemySpellContext& context) override;
    void upgradeCard(int dlvl = 1) override{ lvl += dlvl;}
    void resetUpgrades() override { lvl = 0; };
    int getLVL() override { return lvl; };
    int getCurrentR() { return maxR + dR*lvl; };
    int getDamage() { return damage; }
    bool wasUsed() override { return spellWasUsed; }
};