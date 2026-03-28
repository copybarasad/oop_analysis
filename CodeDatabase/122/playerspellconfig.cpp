#include "playerspellconfig.h"
#include "ibattlecard.h"

PlayerSpellConfig::PlayerSpellConfig(GameModel* gameModel, std::pair<int,int> targetPos)
{
    m_gameModel = gameModel;
    m_targetX = targetPos.first;
    m_targetY = targetPos.second;
}

void PlayerSpellConfig::execute(IBattleCard &card)
{
   card.apply(*this);
}

std::vector<Entity*>& PlayerSpellConfig::getTargets(int spellRadius, int spellArea)
{
    static std::vector<Entity*> result;
    result.clear();

    if (!isTargetInRange(spellRadius)) return result;

    auto& enemies = m_gameModel->getEnemies();

    for (auto& enemy : enemies)
    {
        if (enemy->getXPos() >= m_targetX && enemy->getXPos() < m_targetX + spellArea &&
            enemy->getYPos() >= m_targetY && enemy->getYPos() < m_targetY + spellArea)
        {
            result.push_back(enemy.get());
        }
    }

    auto& tower = m_gameModel->getTower();

    bool hitsTowerX = (m_targetX <= tower.getXPos() + 1) &&
                      (m_targetX + spellArea - 1 >= tower.getXPos());
    bool hitsTowerY = (m_targetY <= tower.getYPos() + 1) &&
                      (m_targetY + spellArea - 1 >= tower.getYPos());

    if (hitsTowerX && hitsTowerY && tower.  isAlive()) {
        result.push_back(&tower);
    }
    return result;
}

bool PlayerSpellConfig::isTargetInRange(int spellRadius)
{
    auto& player = m_gameModel->getPlayers().front();

    int distance = std::max(std::abs(m_targetX - player->getXPos()), std::abs(m_targetY - player->getYPos()));
    if (distance > spellRadius) return false;

    return true;
}

void PlayerSpellConfig::spawnArcher(int xPos, int yPos)
{
    m_gameModel->spawnArcher(xPos, yPos);
}

void PlayerSpellConfig::execute(IUpgradeCard &)
{
    throw std::logic_error("Supports only battle cards");
}
