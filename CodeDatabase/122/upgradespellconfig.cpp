#include "upgradespellconfig.h"
#include "upgradecard.h"
#include "ispellcard.h"

UpgradeSpellConfig::UpgradeSpellConfig(ISpellCard *card) : m_card(card) {}

void UpgradeSpellConfig::execute(IUpgradeCard &card)
{
    card.apply(*this);
}

void UpgradeSpellConfig::execute(IBattleCard&)
{
    throw std::logic_error("UpgradeeSpellConfig supports only upgrade cards");
}
