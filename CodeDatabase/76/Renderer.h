#pragma once
#include "Game.h"
#include "PlayingField.h"
#include "Player.h"
#include "Enemy.h"
#include "SpellHand.h"
#include <iostream>
#include <string>

class Renderer {
public:
    static void DisplayGameInfo(const Game& game);
    static void DisplayGameState(const Game& game);
    static void DisplayStatistics(const Game& game);
    
    static void RenderMainMenu();
    static void RenderPlayerNameInput();
    static void RenderLevelStart(int level);
    static void RenderFieldSizeInput();
    static void RenderBarriersCountInput();
    static void RenderCombatRangeInput();
    
    static void RenderSpellShop(const Player& player);
    static void RenderSpellPurchaseResult(bool success, const std::string& spellName, int manaCost, int currentMana);
    static void RenderInvalidSpellChoice();
    
    static void RenderNoSpellsMessage();
    static void RenderSpellTargetSelection(const std::string& spellName, const std::string& enemyName);
    static void RenderCannotCastOnEnemy();
    static void RenderCannotCastOnTower();
    static void RenderNoTowersAvailable();
    static void RenderInvalidTargetChoice();
    static void RenderSpellCastingCancelled();
    static void RenderSpellCastOnTower(const std::string& playerName, const std::string& spellName, int towerX, int towerY, int towerHp, int towerMaxHp);
    static void RenderInvalidSpellIndex();
    static void RenderCannotCastSpell();
    
    static void RenderGameOver();
    static void RenderEnemyDefeated();
    static void RenderLevelCompleted(int level);
    static void RenderRestartPrompt();
    static void RenderExitMessage();
    
    static void RenderInvalidInput();
    static void RenderInvalidWidthInput();
    static void RenderInvalidHeightInput();
    static void RenderInvalidFieldSize();
    static void RenderTooManyBarriers();
    static void RenderNegativeBarriers();
    static void RenderInvalidCombatRange();
    
    static void RenderRandomSpellFound();
    static void RenderSpellsRemoved(int count);
    
    static void RenderDebugFieldState(const Game& game);
    
    static void RenderCommandMessage(const std::string& message);

    static void ClearScreen();
    static void RenderSeparator();
};
