#ifndef GAME_H
#define GAME_H

#include "GameController.h"
#include <memory>
#include <string>
#include <fstream>

enum class GameState {
    MAIN_MENU,
    PLAYING,
    GAME_OVER,
    EXIT
};

class Game {
private:
    std::unique_ptr<GameController> currentGame;
    GameState currentState;
    bool isRunning;

public:
    Game();
    void run();

private:
    void showMainMenu();
    void processMainMenuInput(char input);
    void startNewGame();
    void gameOverMenu();
    void processGameOverInput(char input);

    bool saveGameToFile(const std::string& filename);
    bool loadGameFromFile(const std::string& filename);
    
    bool loadGameData(std::ifstream& file, GameController& gameController);
    bool validateHeader(std::ifstream& file);
    bool loadPlayerData(std::ifstream& file, GameController& gameController);
    bool loadFieldData(std::ifstream& file, GameController& gameController);
    bool loadEntitiesData(std::ifstream& file, GameController& gameController);
    
    bool savePlayerData(std::ofstream& file);
    bool saveFieldData(std::ofstream& file);
    bool saveEntitiesData(std::ofstream& file);
    bool loadPlayerData(std::ifstream& file);
    bool loadFieldData(std::ifstream& file, int fieldWidth, int fieldHeight);
    bool loadEntitiesData(std::ifstream& file);
};

#endif