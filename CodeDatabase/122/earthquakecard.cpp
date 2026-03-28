#include "earthquakecard.h"
#include "player.h"
#include "gamemodel.h"

EarthQuakeCard::EarthQuakeCard()
    :
    m_spellRadius(SpellConfig::EarthQuakeSpellRadius),
    m_spellDamage(SpellConfig::EarthQuakeDamage),
    m_spellArea(SpellConfig::EarthQuakeArea), m_wasUsed(false)
{}

void EarthQuakeCard::useCard(ISpellConfig& config)
{
    config.execute(*this);
}

void EarthQuakeCard::upgrade(int level)
{
    m_spellArea += level;
    m_upgradeLevel = level;
}

void EarthQuakeCard::clearUpgrades()
{
    m_spellArea = SpellConfig::EarthQuakeArea;
    m_wasUsed = false;
    m_upgradeLevel = 0;
}

void EarthQuakeCard::apply(PlayerSpellConfig &config)
{
    auto& targets = config.getTargets(m_spellRadius, m_spellArea);
    activate(targets);
}

void EarthQuakeCard::apply(EnemySpellConfig &config)
{
    auto& targets = config.getTargets(m_spellRadius, m_spellArea);
    activate(targets);
}

void EarthQuakeCard::activate(std::vector<Entity*>& targets)
{
    m_wasUsed = true;

    if (targets.empty()) return;

    for (auto& target : targets)
    {
        target->takeDamage(m_spellDamage);
    }
}
