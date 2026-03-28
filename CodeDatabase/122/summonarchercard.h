#ifndef SUMMONARCHERCARD_H
#define SUMMONARCHERCARD_H

#include "ibattlecard.h"

class SummonArcherCard : public IBattleCard
{
public:
    SummonArcherCard();

    void useCard(ISpellConfig& config) override;
    void upgrade(int level) override;
    void clearUpgrades() override;
    bool wasUsed() const override { return m_wasUsed; }
    int getUpgradeLevel() const override { return m_upgradeLevel; }

    void apply(PlayerSpellConfig& config) override;
    void apply(EnemySpellConfig& config) override;

private:
    int m_spellRadius;
    int m_countArchers;
    bool m_wasUsed;
    int m_upgradeLevel{0};
};

#endif // SUMMONARCHERCARD_H
