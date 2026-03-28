#ifndef PLAYERSPELLCONFIG_H
#define PLAYERSPELLCONFIG_H

#include "gamemodel.h"
#include "ispellconfig.h"

class PlayerSpellConfig : public ISpellConfig
{
public:
    PlayerSpellConfig(GameModel* gameModel, std::pair<int,int> targetPos);

    void execute(IBattleCard& card) override;
    void execute(IUpgradeCard&) override;

    std::vector<Entity*>& getTargets(int spellRadius, int spellArea);
    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return m_gameModel->getEnemies(); }
    GameBoard& getBoard() { return m_gameModel->getBoard(); }
    std::pair<int, int> getTargetPos() { return {m_targetX, m_targetY}; }
    bool isTargetInRange(int radius);
    void spawnArcher(int xPos, int yPos);

private:
    GameModel* m_gameModel;
    int m_targetX;
    int m_targetY;
};

#endif // PLAYERSPELLCONFIG_H
