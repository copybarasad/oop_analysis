#include "enemyspellconfig.h"
#include "ibattlecard.h"

EnemySpellConfig::EnemySpellConfig(GameModel* gameModel, std::pair<int,int> targetPos)
{
    m_gameModel = gameModel;
    m_targetX = targetPos.first;
    m_targetY = targetPos.second;
}

void EnemySpellConfig::execute(IBattleCard &card)
{
    card.apply(*this);
}

void EnemySpellConfig::execute(IUpgradeCard &)
{
    throw std::logic_error("Supports only battle cards");
}

std::vector<Entity*>& EnemySpellConfig::getTargets(int spellRadius, int spellArea)
{
    static std::vector<Entity*> result;
    result.clear();

    if (!isTargetInRange(spellRadius)) return result;

    auto& players = m_gameModel->getPlayers();

    for (auto& player : players)
    {
        if (player->getXPos() >= m_targetX && player->getXPos() < m_targetX + spellArea &&
            player->getYPos() >= m_targetY && player->getYPos() < m_targetY + spellArea)
        {
            result.push_back(player.get());
        }
    }

    auto& archers = m_gameModel->getArchers();

    for (auto& archer : archers)
    {
        if (archer->getXPos() >= m_targetX && archer->getXPos() < m_targetX + spellArea &&
            archer->getYPos() >= m_targetY && archer->getYPos() < m_targetY + spellArea)
        {
            result.push_back(archer.get());
        }
    }

    return result;
}

bool EnemySpellConfig::isTargetInRange(int spellRadius)
{
    auto& tower = m_gameModel->getTower();

    int distance = std::max(std::abs(m_targetX - tower.getXPos()), std::abs(m_targetY - tower.getYPos()));
    if (distance > spellRadius) return false;

    return true;
}
