#include "summonarchercard.h"
#include "player.h"
#include "gamemodel.h"
#include <iostream>
SummonArcherCard::SummonArcherCard()
    :
    m_spellRadius(SpellConfig::SummonArcherSpellRadius),
    m_countArchers(1),
    m_wasUsed(false) {}

void SummonArcherCard::useCard(ISpellConfig& config)
{
    config.execute(*this);
}

void SummonArcherCard::upgrade(int level)
{
    m_countArchers += level;
    m_upgradeLevel = level;
}

void SummonArcherCard::clearUpgrades()
{
    m_countArchers = SpellConfig::SummonArchersCount;
    m_wasUsed = false;
    m_upgradeLevel = 0;
}

void SummonArcherCard::apply(PlayerSpellConfig &config)
{
    if (!config.isTargetInRange(m_spellRadius)) return;

    auto& board = config.getBoard();

    std::pair<int,int> targetPos = config.getTargetPos();

    for (int i = 0; i < m_countArchers; ++i)
    {
        if (board.isValidCell(targetPos.first + i, targetPos.second))
        {
            config.spawnArcher(targetPos.first + i, targetPos.second);
        }
    }
    m_wasUsed = true;
}

void SummonArcherCard::apply(EnemySpellConfig &)
{
    throw std::logic_error("SummonArcherCard: Enemy spell config not supported yet");
}
