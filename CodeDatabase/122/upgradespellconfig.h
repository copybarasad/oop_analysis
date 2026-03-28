#ifndef UPGRADESPELLCONFIG_H
#define UPGRADESPELLCONFIG_H

#include "ispellconfig.h"

class ISpellCard;

class UpgradeSpellConfig : public ISpellConfig
{
public:
    void execute(IUpgradeCard& card) override;
    void execute(IBattleCard&) override;

    UpgradeSpellConfig(ISpellCard* card);
    ISpellCard* getCard() { return m_card; }

private:
    ISpellCard* m_card;
};

#endif // UPGRADESPELLCONFIG_H
