#include "fireballcard.h"
#include "gamemodel.h"
#include "player.h"

FireBallCard::FireBallCard(int spellRadius, int spellDamage)
    :
    m_spellDamage(spellDamage),
    m_spellRadius(spellRadius),
    m_spellArea(SpellConfig::FireBallArea), m_wasUsed(false)
{}

void FireBallCard::useCard(ISpellConfig& config)
{
    config.execute(*this);
}

void FireBallCard::upgrade(int level)
{
    m_spellRadius += level;
    m_upgradeLevel = level;
}

void FireBallCard::clearUpgrades()
{
    m_spellRadius = SpellConfig::FireBallSpellRadius;
    m_wasUsed = false;
    m_upgradeLevel = 0;
}

void FireBallCard::apply(PlayerSpellConfig &config)
{
    auto& targets = config.getTargets(m_spellRadius, m_spellArea);
    activate(targets);
}

void FireBallCard::apply(EnemySpellConfig &config)
{
    auto& targets = config.getTargets(m_spellRadius, m_spellArea);
    activate(targets);
}

void FireBallCard::activate(std::vector<Entity*>& targets)
{
    if (targets.empty()) return;
    targets.front()->takeDamage(m_spellDamage);
    m_wasUsed = true;
}
