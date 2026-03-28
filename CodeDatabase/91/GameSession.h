#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include "Game.h"
#include "SaveLoad.h"
#include "Logger.h"
#include <memory>
#include <iostream>
#include <algorithm>

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETE,
    QUITTING
};

class GameSession {
    private:
        GameState currentState;
        std::unique_ptr<Game> currentGame;
        int currentLevel;
        
        std::unique_ptr<Logger> logger_;
        std::string default_control_config_path_ = "controls.json";
        std::string current_control_config_path_;

        void displayMainMenu();
        void displayGameOverMenu();
        char getPlayerChoice();

        void startNewGame(int fieldWidth = 15, int fieldHeight = 15);

        void startNextLevel();

        void runGameWithConfig(const std::string& config_path);

        // === JSON ===
        void loadGame();

    public:
        GameSession();
        ~GameSession() = default;

        void run();
};

#endif