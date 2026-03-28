#ifndef GAME_H
#define GAME_H

#ifdef _WIN32
#define NOMINMAX
#endif

#include "game_field.h"
#include "player.h"
#include "enemy.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>

class Game {
public:
    enum class GameState {
        MAIN_MENU,
        PLAYING,
        GAME_OVER,
        LEVEL_COMPLETE
    };

    Game();
    ~Game();

    void run();

private:
    void clearScreen() const;
    void displayField() const;
    void displayStats() const;
    void waitForEnter() const;

    void mainMenu();
    void startNewGame();
    void loadGame();
    void saveGame();
    void gameLoop();
    void processPlayerTurn();
    void processEnemyTurn();

    void loadLevel(int level);
    bool isLevelComplete() const;
    bool isGameOver() const;

    void handleMoveCommand(char direction);
    void handleCastSpellCommand(int spellIndex);
    void handleSwitchModeCommand();

    Position findClosestEnemyInRange(int range) const;

    void serialize(std::ofstream& file) const;
    void deserialize(std::ifstream& file);
    void handleLevelComplete();

    GameState state_;
    GameField field_;
    std::unique_ptr<Player> player_;
    std::vector<Enemy> enemies_;
    int currentLevel_;
    int score_;
    bool running_;

    static const std::string SAVE_FILE;
    static const int MAX_LEVELS = 3;  
};

#endif