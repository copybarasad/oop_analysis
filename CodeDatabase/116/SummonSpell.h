#pragma once
#include "SpawnSpellContext.h"
#include "ISpawnSpell.h"
#include <iostream>
#include <cmath>

class SummonSpell : public ISpawnSpell{
private:
    int maxR;
    int lvl;
    bool spellWasUsed;
public:
    SummonSpell(int lvl = 0, int maxR = 1) : maxR(maxR), lvl(lvl), spellWasUsed(false){};
    ~SummonSpell() override = default;
    std::string getName() override;
    void cast(ISpellContext& config) override;
    int getLVL() override { return lvl; };
    void apply(SpawnSpellContext& context) override;
    void upgradeCard(int dlvl = 1) override{ lvl += dlvl;}
    void resetUpgrades() override { lvl = 0; }
    bool wasUsed() override { return spellWasUsed; }
};