#include "GameManager.h"
#include "GameConstants.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

GameManager::GameManager() 
    : currentPhase(GamePhase::MAIN_MENU)
    , gameRunning(true) {
}

void GameManager::cleanup() {
    gameController.reset();
}

void GameManager::run() {
    while (gameRunning) {
        switch (currentPhase) {
            case GamePhase::MAIN_MENU:
                showMainMenu();
                break;
                
            case GamePhase::IN_GAME:
                if (gameController) {
                    gameController->runGame();
                    
                    currentPhase = GamePhase::GAME_OVER;
                }
                break;
                
            case GamePhase::GAME_OVER:
                showGameOverMenu();
                break;
                
            case GamePhase::SAVE_MENU:
                handleSaveGame();
                currentPhase = GamePhase::IN_GAME;
                break;
                
            case GamePhase::LOAD_MENU:
                if (handleLoadGame()) {
                    currentPhase = GamePhase::IN_GAME;
                } else {
                    currentPhase = GamePhase::MAIN_MENU;
                }
                break;
                
            case GamePhase::QUIT:
                gameRunning = false;
                break;
        }
    }
}

void GameManager::showMainMenu() {
    Renderer::clearScreen();
    std::cout << "=== LEGEND OF SEGFAULT ===\n\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Save Information\n";
    std::cout << "4. Delete Save\n";
    std::cout << "5. Exit\n\n";
    
    int choice = InputHandler::getValidatedIntegerInput("Select option: ", 1, 5);
    
    switch (choice) {
        case 1:
            startNewGame();
            currentPhase = GamePhase::IN_GAME;
            break;
            
        case 2:
            currentPhase = GamePhase::LOAD_MENU;
            break;
            
        case 3:
            saveMenu.showSaveInfo();
            break;
            
        case 4:
            saveMenu.showDeleteMenu();
            break;
            
        case 5:
            currentPhase = GamePhase::QUIT;
            break;
    }
}

void GameManager::startNewGame() {
    cleanup();
    
    std::cout << "\n=== NEW GAME ===\n\n";
    
    gameController = std::make_unique<GameController>();
    gameController->startNewGame();
    std::cout << "\nStarting game...\n";
}


void GameManager::showGameOverMenu() {
    Renderer::clearScreen();
    std::cout << "=== GAME OVER ===\n\n";
    std::cout << "1. Restart Game\n";
    std::cout << "2. Return to Main Menu\n";
    std::cout << "3. Exit Game\n\n";
    
    int choice = InputHandler::getValidatedIntegerInput("Select option: ", 1, 3);
    
    switch (choice) {
        case 1:
            restartGame();
            break;
        case 2:
            cleanup();
            currentPhase = GamePhase::MAIN_MENU;
            break;
        case 3:
            currentPhase = GamePhase::QUIT;
            break;
    }
}

void GameManager::handleSaveGame() {
    if (gameController) {
        try {
            SaveData data = gameController->getSaveData();
            saveMenu.showSaveMenu(data);
        } catch (const std::exception& e) {
            std::cerr << "Error getting save data: " << e.what() << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
}

bool GameManager::handleLoadGame() {
    SaveData data;
    if (saveMenu.showLoadMenu(data)) {
        try {
            cleanup();
            gameController = std::make_unique<GameController>();
            gameController->loadFromSave(data);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading game: " << e.what() << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            cleanup();
        }
    }
    return false;
}

void GameManager::restartGame() {
    gameController = std::make_unique<GameController>();
    gameController->startNewGame();
    std::cout << "\nRestarting game...\n";
    currentPhase = GamePhase::IN_GAME;
}
