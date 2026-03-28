#ifndef EARTHQUAKECARD_H
#define EARTHQUAKECARD_H

#include "ibattlecard.h"

class EarthQuakeCard : public IBattleCard
{
public:
    EarthQuakeCard();

    void useCard(ISpellConfig& config) override;
    void upgrade(int level) override;
    void clearUpgrades() override;
    bool wasUsed() const override { return m_wasUsed; }
    int getUpgradeLevel() const override { return m_upgradeLevel; }

    void apply(PlayerSpellConfig& config) override;
    void apply(EnemySpellConfig& config) override;

private:
    void activate(std::vector<Entity*>& targets);
    int m_spellRadius;
    int m_spellDamage;
    int m_spellArea;
    bool m_wasUsed;
    int m_upgradeLevel{0};
};

#endif // EARTHQUAKECARD_H
