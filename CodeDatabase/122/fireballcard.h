#ifndef FIREBALLCARD_H
#define FIREBALLCARD_H

#include "ibattlecard.h"

class FireBallCard : public IBattleCard
{
public:
    FireBallCard(int spellRadius=SpellConfig::FireBallSpellRadius, int spellDamage=SpellConfig::FireBallDamage);

    void useCard(ISpellConfig& config) override;
    void upgrade(int level) override;
    void clearUpgrades() override;
    bool wasUsed() const override { return m_wasUsed; }
    int getUpgradeLevel() const override { return m_upgradeLevel; }

    void apply(PlayerSpellConfig& config) override;
    void apply(EnemySpellConfig& config) override;

private:
    void activate(std::vector<Entity*>& targets);

    int m_spellDamage;
    int m_spellRadius;
    int m_spellArea;
    bool m_wasUsed;
    int m_upgradeLevel{0};
};

#endif // FIREBALLCARD_H
