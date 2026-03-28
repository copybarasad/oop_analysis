#pragma once
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>
#include <unordered_map>

#include "Command.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "GameState.h"

enum class GamePhase {
    Menu,
    GameStart,
    LevelStart,
    PlayerTurn,
    EnemiesTurn,
    AlliesTurn, //d
    EnemyBaseTurn, //d
    LevelComplete,
    GameOver,
    Exit
};

class Game {
private:
    struct ControlSettings {
        char moveUp{'w'};
        char moveDown{'s'};
        char moveLeft{'a'};
        char moveRight{'d'};
        char switchMode{'m'};
        char castSpell{'c'};
        char saveGame{'v'};
        char quitToMenu{'q'};
    };

    Field field;
    bool running;
    int currentLevel;
    int initialEnemyCount;
    int enemiesKilled;
    GamePhase currentPhase;
    static const std::string SAVE_FILE;
    static const std::string CONTROLS_FILE;
    ControlSettings controls;

    // Game loop phases
    void processMenu();
    void processGameStart();
    void processLevelStart();
    void processPlayerTurn();
    void processAlliesTurn(); // Placeholder for future allies
    void processEnemiesTurn();
    void processEnemyBaseTurn(); // Placeholder for future bases/towers
    void processLevelComplete();
    void processGameOver();
    
    // Game state management
    void initializeNewGame();
    void initializeLevel(int level);
    bool checkLevelComplete() const;
    bool checkGameOver() const;
    void tryGiveNewSpell();
    
    // UI
    void showStats() const;
    void showHand() const;
    void displayGameScreen() const;
    
    // Save/Load
    bool saveGame() const;
    bool loadGame();
    GameState captureGameState() const;
    void restoreGameState(const GameState& state);
    
    // Helper methods
    void pause(const std::string& message = "Press Enter to continue...") const;
    void setDefaultControls();
    void loadControls();
    bool parseControlsFile(ControlSettings& outSettings, std::string& error) const;
    bool validateControls(const ControlSettings& settings, std::string& error) const;

public:
    Game();
    ~Game() = default;
    
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(Game&& other) = delete;
    
    void run();
    std::unordered_map<std::string, char> getControlsForTest() const;
    bool applyCommand(const Command& cmd);
    void handlePostPlayerAction(bool turnUsed);
    void render() const;
};
