#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "field.h"
#include "player.h"
#include "level_manager.h"
#include "upgrade_system.h"
#include "game_state.h"
#include "save_system.h"
#include <string>
#include <vector>
#include <map>
#include "command.h"

class GameManager {
public:
    GameManager();
    ~GameManager();

    void run();

    void startNewGame();

    void loadGame(const std::string& saveFile);

    void saveGame(const std::string& saveFile);
    
    void reloadConfig(const std::string& configFile);
    
    enum class GameState_Enum {
        MainMenu,
        Playing,
        LevelComplete,
        GameOver,
        Exit
    };

private:
    
    Field* field_;
    Player* player_;
    LevelManager* levelManager_;
    UpgradeSystem* upgradeSystem_;
    SaveSystem* saveSystem_;

    std::vector<Entity*> allEntities_;
    
    GameState_Enum gameState_;
    int currentLevel_;
    bool playerWon_;

    // Current key bindings used by GameManager for player input
    std::map<char, CommandType> keyBindings_;

    // Ensure we have sensible defaults if config not available
    void loadDefaultBindings();

    void showMainMenu();
    void playLevel();
    void handlePlayerTurn();
    void handleAllyTurns();
    void handleEnemyTurns();
    void handleBuildingTurns();
    void checkLevelConditions();
    void handleLevelComplete();
    bool showGameOver();

    void serializeGameState(GameState& state);
    void deserializeGameState(const GameState& state);

    char getPlayerCommand();
    void displayGameInfo();

    void cleanup();
};

#endif
