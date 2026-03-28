#ifndef CONSOLE_RENDER_ADAPTER_H
#define CONSOLE_RENDER_ADAPTER_H

#include "IRenderAdapter.h"
#include "gamefield.h"
#include "player.h"
#include "enemy.h"
#include "enemytower.h"
#include "EnemyManager.h"
#include <vector>
#include <string>

class ConsoleRenderAdapter : public IRenderAdapter {
public: 
    void renderGame(const GameField& field, const Player& player, const Enemy& enemy,
                   const std::vector<EnemyTower>& towers, const EnemyManager& enemyManager) override;
    void renderHelp() override;
    void renderUI(const GameField& field, const Player& player, const Enemy& enemy,
                 const std::vector<EnemyTower>& towers, const EnemyManager& enemyManager) override;
     
    void renderField(const GameField& field) const;
    void renderPlayerStatus(const Player& player) const;
    void renderEnemyInfo(const Enemy& enemy) const;
    void renderTowersInfo(const std::vector<EnemyTower>& towers) const;
    void renderProgressInfo(const EnemyManager& enemyManager) const;
    
private:
    std::string getCellSymbol(const GameField& field, int x, int y) const;
};

#endif