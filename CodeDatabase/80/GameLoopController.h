#pragma once
#ifndef GAMELOOPCONTROLLER_H
#define GAMELOOPCONTROLLER_H

#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include "SimpleConsoleInputProcessor.h" 

class Game;
class ICommand;

template<typename InputProcessorType>
class GameLoopController {
private:
    Game* game;
    std::unique_ptr<InputProcessorType> inputProcessor;
    bool running;
    bool inMenu;
    bool inLevelUpMenu;
    bool inLoadMenu;

    void processMenuInput(const std::string& input) {
        if (input == "1" || input == "menu_start") {
            startNewGame();
        }
        else if (input == "2" || input == "menu_load") {
            showLoadMenu();
        }
        else if (input == "3" || input == "menu_exit") {
            running = false;
        }
        else if (input == "invalid_menu_input") {
            game->showMainMenu();
        }
    }

    void processLevelUpInput(const std::string& input) {
        if (input == "1" || input == "2" || input == "3" || input == "4") {
            int choice = std::stoi(input);

            std::string msg;
            switch (choice) {
            case 1: msg = "Max Health upgraded!"; break;
            case 2: msg = "Base Damage upgraded!"; break;
            case 3: msg = "Random Spell upgraded!"; break;
            case 4: msg = "New Spell added!"; break;
            }
            inputProcessor->showMessage(msg);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            if (game) {

                game->AdvanceToNextLevel();
                inLevelUpMenu = false;

                game->DisplayGameState();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
        else {
            inputProcessor->showMessage("Invalid choice! Using default upgrade.");
            if (game) {
                game->AdvanceToNextLevel();
                inLevelUpMenu = false;
                game->DisplayGameState();
            }
        }
    }

    void returnToMainMenu() {
        inMenu = true;
        inLevelUpMenu = false;
        inLoadMenu = false;

        setInputProcessorMenuMode(true);

        if (game) {
            game->returnToMainMenu();
        }
    }

    void startNewGame() {
        if (!game) return;

        std::cout << "Starting new game..." << std::endl;
        inMenu = false;
        inLevelUpMenu = false;
        inLoadMenu = false;

        setInputProcessorMenuMode(false);

        game->StartNewGame();
    }

    void setInputProcessorMenuMode(bool menuMode) {
        SimpleConsoleInputProcessor* processor =
            dynamic_cast<SimpleConsoleInputProcessor*>(inputProcessor.get());
        game->inMainMenu = false;
        if (processor) {
            processor->setMenuMode(menuMode);

            std::cout << "DEBUG: InputProcessor menu mode set to "
                << (menuMode ? "ON (menu)" : "OFF (game)") << std::endl;
        }
        else {
            std::cout << "DEBUG: Cannot cast to SimpleConsoleInputProcessor!" << std::endl;
        }
    }

    void runGameLoop() {
        while (running && !inMenu && !inLevelUpMenu && !inLoadMenu &&
            game) {
            if (game->isLevelComplete()) {
                if (game->hasNextLevel()) {
                    handleLevelComplete();
                    continue; 
                }
                else {
                    handleGameVictory();
                    returnToMainMenu();
                    break;
                }
            }

            if (game->isGameOver()) {
                handleGameOver();
                returnToMainMenu();
                break;
            }
            game->inMainMenu = false;
            game->DisplayGameState();

            std::unique_ptr<ICommand> command = inputProcessor->getNextCommand();

            if (command) {
                bool shouldContinue = command->execute(
                    game->getHero(),
                    game->getMap(),
                    *game
                );

                if (!shouldContinue) {
                    returnToMainMenu();
                    break;
                }
            }

            if (game->isGameRunning() && !game->isLevelComplete() && !game->isGameOver()) {
                game->ProcessEnemiesTurn();
                game->ProcessStructuresTurn();
            }
        }
    }

    void handleLevelComplete() {
        if (!game) return;
        GameRenderData data = game->GetRenderData();
        inLevelUpMenu = true;
        game->levelCompleted = true;
        game->DisplayGameState();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::unique_ptr<ICommand> command = inputProcessor->getNextCommand();
        if (command) {
            processLevelUpInput(command->getName());
        }
    }


    void handleGameVictory() {
        if (!game) return;

        std::cout << "\n=== VICTORY! ===" << std::endl;
        std::cout << "You completed all levels!" << std::endl;

        game->DisplayGameState();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    void handleGameOver() {
        if (!game) return;

        std::cout << "\n=== GAME OVER ===" << std::endl;
        game->DisplayGameState();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    void showLoadMenu() {
        inLoadMenu = true;
        inMenu = false;

        SimpleConsoleInputProcessor* processor =
            dynamic_cast<SimpleConsoleInputProcessor*>(inputProcessor.get());
        if (processor) {
            processor->setLoadMenuMode(true);
        }

        if (game) {
            game->showLoadMenu();
        }

        handleLoadMenuSelection();
    }

    void handleLoadMenuSelection() {
        if (!game) return;

        std::unique_ptr<ICommand> command = inputProcessor->getNextCommand();
        if (command) {
            bool success = command->execute(game->getHero(), game->getMap(), *game);

            if (success) {
                std::cout << "DEBUG: Load successful, entering game mode..." << std::endl;   
                inMenu = false;       
                inLevelUpMenu = false;

                SimpleConsoleInputProcessor* processor =
                    dynamic_cast<SimpleConsoleInputProcessor*>(inputProcessor.get());
                if (processor) {
                    processor->setMenuMode(false);
                    processor->setLoadMenuMode(false);
                    std::cout << "DEBUG: InputProcessor set to game mode" << std::endl;
                }

                std::cout << "Starting loaded game..." << std::endl;
                inLoadMenu = false;
                game->inLoadMenu = false;
                runGameLoop();  

            }
            else {
                std::cout << "DEBUG: Load failed or cancelled" << std::endl;

                inLoadMenu = false;
                inMenu = true;

                SimpleConsoleInputProcessor* processor =
                    dynamic_cast<SimpleConsoleInputProcessor*>(inputProcessor.get());
                if (processor) {
                    processor->setMenuMode(true);
                    processor->setLoadMenuMode(false);
                }

                if (game) {
                    game->showMainMenu();
                }
            }
        }
    }


public:
    GameLoopController(Game* gameInstance)
        : game(gameInstance),
        inputProcessor(new InputProcessorType()),
        running(false),
        inMenu(true),
        inLevelUpMenu(false),
        inLoadMenu(false) {

        setInputProcessorMenuMode(true);
    }

    void start() {
        if (!game) return;

        running = true;

        if (game) {
            game->showMainMenu();
        }

        while (running) {
            if (inMenu) {
                std::unique_ptr<ICommand> command = inputProcessor->getNextCommand();
                if (command) {
                    processMenuInput(command->getName());
                }
            }
            else if (inLevelUpMenu) {
                std::unique_ptr<ICommand> command = inputProcessor->getNextCommand();
                if (command) {
                    processLevelUpInput(command->getName());
                }
            }
            else if (inLoadMenu) {
                std::unique_ptr<ICommand> command = inputProcessor->getNextCommand();
                if (command) {
                }
            }
            else {
                
                runGameLoop();
            }
        }

        std::cout << "\nGame ended. Thanks for playing!" << std::endl;
    }

    bool isRunning() const { return running; }
    bool isInMenu() const { return inMenu; }
};

#endif