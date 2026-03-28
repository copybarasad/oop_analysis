#include "Game.h"
#include <iostream>

Game::Game() 
    : currentState(GameState::MAIN_MENU), isRunning(true) {}

void Game::run() {
    std::cout << "=== BEAST HUNTER ===\n";
    
    while (isRunning) {
        switch (currentState) {
            case GameState::MAIN_MENU:
                showMainMenu();
                break;
            case GameState::PLAYING:
                if (currentGame) {
                    currentGame->runGame();

                    if (!currentGame->getPlayer().getIsAlive()) {
                        currentState = GameState::GAME_OVER;
                    } else {
                        saveGameToFile("saveGame.txt");
                        currentState = GameState::MAIN_MENU;
                    }
                }
                break;
            case GameState::GAME_OVER:
                gameOverMenu();
                break;
            case GameState::EXIT:
                isRunning = false;
                break;
        }
    }
    std::cout << "Thank you for playing!\n";
}

void Game::showMainMenu() {
    std::cout << "\n=== MAIN MENU ===\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose option: ";
    
    char input;
    std::cin >> input;
    processMainMenuInput(input);
}

void Game::processMainMenuInput(char input) {
    switch (input) {
        case '1':
            startNewGame();
            break;
        case '2':
            if (loadGameFromFile("saveGame.txt")) {
                currentState = GameState::PLAYING;
            } else {
                std::cout << "Failed to load game. Starting new game...\n";
                startNewGame();
            }
            break;
        case '3':
            currentState = GameState::EXIT;
            break;
        default:
            std::cout << "Invalid option!\n";
            break;
    }
}

void Game::startNewGame() {
    try {
        currentGame.reset(new GameController(15, 15, false));
        currentState = GameState::PLAYING;
        std::cout << "New game started!\n";
    } catch (const std::exception& e) {
        std::cout << "Error starting game: " << e.what() << "\n";
        currentState = GameState::MAIN_MENU;
    }
}

void Game::gameOverMenu() {
    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "1. Restart Game\n";
    std::cout << "2. Main Menu\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose option: ";
    
    char input;
    std::cin >> input;
    processGameOverInput(input);
}

void Game::processGameOverInput(char input) {
    switch (input) {
        case '1':
            startNewGame();
            break;
        case '2':
            currentState = GameState::MAIN_MENU;
            break;
        case '3':
            currentState = GameState::EXIT;
            break;
        default:
            std::cout << "Invalid option!\n";
            break;
    }
}