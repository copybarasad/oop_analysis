#ifndef ENEMYSPELLCONFIG_H
#define ENEMYSPELLCONFIG_H

#include "gamemodel.h"
#include "ispellconfig.h"

class EnemySpellConfig : public ISpellConfig
{
public:
    EnemySpellConfig(GameModel* gameModel, std::pair<int,int> targetPos);

    void execute(IBattleCard& card) override;
    void execute(IUpgradeCard&) override;

    std::vector<Entity*>& getTargets(int spellRadius, int spellArea);
    bool isTargetInRange(int radius);

private:
    GameModel* m_gameModel;
    int m_targetX;
    int m_targetY;
};

#endif // ENEMYSPELLCONFIG_H
