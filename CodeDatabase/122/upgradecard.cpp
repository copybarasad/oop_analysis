#include "upgradecard.h"
#include <iostream>

UpgradeCard::UpgradeCard() : m_upgradeLevel(1), m_wasUsed(false) {}

void UpgradeCard::useCard(ISpellConfig& config)
{
    config.execute(*this);
}

void UpgradeCard::upgrade(int level)
{
    m_upgradeLevel += (level / 2) + 1;
}

void UpgradeCard::clearUpgrades()
{
    m_wasUsed = false;
}

void UpgradeCard::apply(UpgradeSpellConfig &config)
{
    m_wasUsed = true;
    config.getCard()->upgrade(m_upgradeLevel);
}
