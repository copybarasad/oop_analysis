#ifndef UPGRADECARD_H
#define UPGRADECARD_H

#include "iupgradecard.h"

class UpgradeCard : public IUpgradeCard
{
public:
    UpgradeCard();

    void useCard(ISpellConfig& config) override;
    void upgrade(int level) override;
    void clearUpgrades() override;
    bool wasUsed() const override { return m_wasUsed; }
    int getUpgradeLevel() const override { return m_upgradeLevel; }

    void apply(UpgradeSpellConfig& config) override;

private:
    int m_upgradeLevel;
    bool m_wasUsed;
};

#endif // UPGRADECARD_H
