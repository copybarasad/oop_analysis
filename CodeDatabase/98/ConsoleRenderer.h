#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "field.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "hand.h"
#include "GameTypes.h"
#include <vector>
#include <string>

class ConsoleRenderer {
public:
    ConsoleRenderer() = default;

    void RenderField(const Field& field, int playerX, int playerY, const std::vector<std::pair<int,int>>& enemyPositions);
    void RenderMenu(const Player& player, const Hand& hand, int playerX, int playerY);
    void RenderPlayerStats(const Player& player, const Hand& hand);
    
    void RenderAttackResult(const AttackResult& result);
    void RenderSpellResult(const SpellResult& result);
    void RenderSearchResults(const std::vector<EnemyInfo>& enemies, int playerX, int playerY);
    void RenderEnemyAttacks(const std::vector<std::string>& attacks);
    void RenderGameMessage(const std::string& message);

    void RenderUpgradeMenu();
    void RenderSaveLoadMessage(const std::string& message);
    void RenderGameOver(bool playerWon);
    
    void ShowSeparator();

    void RenderEnemyInfo(const EnemyInfo& enemy);
};

#endif