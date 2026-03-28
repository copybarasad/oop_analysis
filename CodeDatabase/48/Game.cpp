#include "lib/Game.h"
#include "lib/GameInstructions.h"
#include "lib/GameSession.h"
#include "lib/SaveSystem.h"
#include <iostream>

// Объявление внешней функции
extern int getInputInt(const std::string& prompt, int min, int max);

Game::Game() : isRunning(true) {
}

void Game::start() {
    std::cout << "=== RPG GAME WITH SPELL SYSTEM ===\n\n";
    
    while (isRunning) {
        showMainMenu();
        
        int choice = getInputInt("Select option (1-4): ", 1, 4);
        
        switch (choice) {
            case 1:
                startNewGame();
                break;
            case 2:
                loadSavedGame();
                break;
            case 3:
                GameInstructions::displayInstructions();
                break;
            case 4:
                exit();
                break;
        }
    }
}

void Game::startNewGame() {
    GameSession session;
    session.startNewGame();
    
    if (session.isActive()) {
        session.processGameLoop();
    }
    
    handleGameOver();
}

void Game::loadSavedGame() {
    auto saves = SaveSystem::getAvailableSaves();
    if (saves.empty()) {
        std::cout << "❌ No save files found! Starting new game instead.\n";
        startNewGame();
        return;
    }
    
    std::cout << "\n=== LOAD SAVED GAME ===\n";
    std::cout << "Available saves:\n";
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << i + 1 << ". " << saves[i] << "\n";
    }
    
    int choice = getInputInt("Select save to load (1-" + std::to_string(saves.size()) + "): ", 1, saves.size());
    
    GameSession session;
    session.loadSavedGame(saves[choice - 1]);
    
    if (session.isActive()) {
        session.processGameLoop();
    }
    
    handleGameOver();
}

void Game::exit() {
    isRunning = false;
    std::cout << "Thank you for playing! Goodbye!\n";
}

void Game::showMainMenu() {
    std::cout << "\n" << std::string(40, '=') << "\n";
    std::cout << "           MAIN MENU\n";
    std::cout << std::string(40, '=') << "\n";
    std::cout << "1. Start New Game\n";
    std::cout << "2. Load Saved Game\n";
    std::cout << "3. Show Instructions\n";
    std::cout << "4. Exit Game\n";
    std::cout << std::string(40, '=') << "\n";
}

void Game::handleGameOver() {
    if (!shouldRestart()) {
        return;
    }
    
    std::cout << "\n" << std::string(40, '=') << "\n";
    std::cout << "          GAME OVER\n";
    std::cout << std::string(40, '=') << "\n";
    
    int choice = getInputInt("What would you like to do?\n1. Play Again\n2. Return to Main Menu\n3. Exit Game\nSelect option (1-3): ", 1, 3);
    
    switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2:
            // Return to main menu
            break;
        case 3:
            exit();
            break;
    }
}

bool Game::shouldRestart() const {
    return isRunning;
}