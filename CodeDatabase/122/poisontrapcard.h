#ifndef POISONTRAPCARD_H
#define POISONTRAPCARD_H

#include <memory>
#include "ibattlecard.h"
#include "poisontrap.h"

class PoisonTrapCard : public IBattleCard
{
public:
    PoisonTrapCard();

    void useCard(ISpellConfig& config) override;
    void upgrade(int level) override;
    void clearUpgrades() override;
    bool wasUsed() const override { return m_wasUsed; }
    int getUpgradeLevel() const override { return m_upgradeLevel; }

    void apply(PlayerSpellConfig& config) override;
    void apply(EnemySpellConfig& config) override;

private:
    int m_trapDamage;
    bool m_wasUsed;
    int m_upgradeLevel;
};

#endif // POISONTRAPCARD_H
