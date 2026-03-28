#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "GameField.h"
#include "LevelManager.h"
#include "SaveLoadSystem.h"
#include "Spell.h"
#include <memory>

enum class GamePhase {
    MENU,
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER,
    EXITING
};

class Game {
public:
    Game();
    
    void Run();
    
private:
    std::unique_ptr<Player> player;
    std::unique_ptr<GameField> field;
    std::unique_ptr<LevelManager> levelManager;
    std::unique_ptr<SaveLoadSystem> saveLoadSystem;
    
    GamePhase currentPhase;
    int currentTurn;
    
    void initializeGame();
    void showMainMenu();
    void startNewGame();
    void loadGame();
    void continueGame();
    
    void gameLoop();
    void processTurn();
    void handlePlayerInput();
    void updateAllies();
    void updateEnemies();
    void updateBuildings();
    void updateTowers();
    void checkTraps();
    void removeDeadEntities();
    void checkLevelCompletion();
    void checkGameOver();
    
    void displayGameState();
    void displaySpells();
    
    void handleMovement(char direction);
    void handleAttack();
    void handleSpellCast(const std::string& input);
    void handleSwitchMode();
    void handleBuySpell();
    void handleSaveGame();
    
    void showGameOverMenu();
    void showLevelCompleteScreen();
    void transitionToNextLevel();
    
    bool askYesNo(const std::string& question);
};

#endif

