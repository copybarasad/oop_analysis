#pragma once
#include "Game.h"
#include "Input.h"
#include <fstream>
#include "Logger.h"
#include "Types.h"
#include <iostream>
#include <vector>
#include "JsonValidator.h"
#include "GameController.h"
#include "KeyboardInputReader.h"
#include "GameVisualizer.h"
#include "ConsoleRenderer.h"

class GameManager {
private:
    AppState state = DEFAULT_APP_STATE;
    int currentLevelIndex = DEFAULT_LEVEL_INDEX;
    std::vector<LevelConfig> levels{
        {2, 2, 20, 10},
        {3, 2, 30, 20},
        {5, 3, 60, 25}
    };

public:
    void run() {
        while (state != AppState::Exit) {
            switch (state) {
            case AppState::MainMenu:
                showMainMenu();
                break;
            case AppState::InGame:
                startGame(false);
                state = AppState::MainMenu;
                break;
            case AppState::Exit:
                break;
            }
        }
    }

private:
    void showMainMenu() {
        std::cout << "\n===== ROGUELIKE =====\n";
        std::cout << "1 - Start New Game\n";
        std::cout << "2 - Load Game\n";
        std::cout << "3 - Exit\n";
        std::cout << "> ";

        while (state == AppState::MainMenu) {
            char choice = getInput();

            switch (choice) {
                case '1': {
                    state = AppState::InGame;
                    currentLevelIndex = 0;
                    startGame(false);
                    break;
                }
                case '2': {
                    state = AppState::InGame;
                    currentLevelIndex = 0;
                    startGame(true);
                    break;
                }
                case '3': {
                    state = AppState::Exit;
                    break;
                }
                default: {
                    state = AppState::MainMenu;
                    break;
                }
            }
        }
    }

    void startGame(bool loadFromFile) {
        bool loaded = loadFromFile;

        json playerState;

        while (currentLevelIndex < levels.size()) {
            const LevelConfig& cfg = levels[currentLevelIndex];

            Game game(cfg.width, cfg.height, cfg.numEnemies, cfg.numTowers, 20);
            
            KeyboardInputReader reader;
            GameController<KeyboardInputReader> controller(reader, game);
            ConsoleRenderer renderer;
            GameVisualizer<ConsoleRenderer> visualizer(game);
            
            game.setCurrentLevelIndex(currentLevelIndex);
            
            game.setCurrentLevelIndex(currentLevelIndex);
            if (loaded) {
                std::ifstream file("save.json");
                if (file.is_open()) {
                    try {
                        json j;
                        file >> j;
                        JsonValidator::validateGame(j);
                        game.deserialize(j);
                        Logger::log("Game loaded from save.json");
                    }
                    catch (const std::exception& e) {
                        Logger::log(std::string("[Error]: Invalid save.json: ") + e.what());
                        Logger::log("[Warning]: Starting new game instead");
                    }
                    catch (...) {
                        Logger::log("[Error]: Corrupted save.json (unknown exception). Starting new game.");
                    }
                }
                else {
                    Logger::log("[Error]: save.json not found, starting new game.");
                }
                loaded = false;
            }

            if (!playerState.is_null()) {
                game.getPlayer().deserialize(playerState);
            }

            game.start();
            visualizer.update();
            
            while (game.is_running()) {
                controller.process();
                visualizer.update();
                
                if (!game.getPlayer().isAlive()) {
                    std::cout << "Player has died! Game Over!\n";
                    Logger::log("Game over. Reason: Player died");
                    game.stop();
                    break;
                }
                
                if (game.allEnemiesDead()) {
                    visualizer.setMessage("All enemies defeated! You win!");
                    visualizer.update();
                    game.stop();
                    break;
                }
            }
            

            if (!game.getPlayer().isAlive()) {
                state = AppState::MainMenu;
                break;
            }

            currentLevelIndex++;

            if (currentLevelIndex < levels.size()) {
                game.getPlayer().restoreHealth();
                game.getPlayer().getHand().removeHalfRandomSpells();
                playerState = game.getPlayer().serialize();
                std::cout << "Level " << (currentLevelIndex + 1) << " start!\n";
            }
            else {
                std::cout << "All levels completed!\n";
            }
        }
    }
};
