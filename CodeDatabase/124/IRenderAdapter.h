#ifndef IRENDER_ADAPTER_H
#define IRENDER_ADAPTER_H

#include "gamefield.h"
#include "player.h"
#include "enemy.h"
#include "enemytower.h"
#include "EnemyManager.h"
#include <vector>

class IRenderAdapter {
public:
    virtual ~IRenderAdapter() = default;
    
    virtual void renderGame(const GameField& field, const Player& player, const Enemy& enemy,
                          const std::vector<EnemyTower>& towers, const EnemyManager& enemyManager) = 0;
    virtual void renderHelp() = 0;
    virtual void renderUI(const GameField& field, const Player& player, const Enemy& enemy,
                         const std::vector<EnemyTower>& towers, const EnemyManager& enemyManager) = 0;
};

#endif