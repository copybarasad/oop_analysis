#pragma once
#include "UpgradeSpellContext.h"
#include "IUpgradeSpell.h"

class EnhancingSpell : public IUpgradeSpell{
private:
    int lvl;
    bool spellWasUsed;
public:
    EnhancingSpell(int lvl = 0):lvl(lvl), spellWasUsed(false){};
    ~EnhancingSpell() override = default;
    std::string getName() override;
    void cast(ISpellContext& config) override;
    int getLVL() override { return lvl; };
    void apply(UpgradeSpellContext& context);
    void upgradeCard(int dlvl = 1) override{ lvl += dlvl;}
    void resetUpgrades() override { lvl = 0; }
    bool wasUsed() override { return spellWasUsed; }
};