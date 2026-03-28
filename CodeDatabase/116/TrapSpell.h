#pragma once
#include "ISpawnSpell.h"
#include <iostream>
#include <cmath>

class TrapSpell : public ISpawnSpell{
private:
    int maxR;
    int damage;
    int Ddamage;
    int lvl;
    bool spellWasUsed;
public:
    TrapSpell(int lvl = 0, int maxR = 1, int damage = 25, int Ddamage = 3):maxR(maxR), damage(damage), Ddamage(Ddamage), spellWasUsed(false), lvl(lvl){};
    ~TrapSpell() override = default;
    std::string getName() override;
    void cast(ISpellContext& context) override;
    int getLVL() override { return lvl; };
    void apply(SpawnSpellContext& context) override;
    void upgradeCard(int dlvl = 1) override{ lvl += dlvl;}
    void resetUpgrades() override { lvl = 0; }
    int getCurrentDamage() { return damage + Ddamage*lvl; }
    bool wasUsed() override { return spellWasUsed; }
};