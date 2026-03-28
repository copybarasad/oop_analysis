#include "Game.h"
#include <iostream>
#include <limits>

Game::Game() : currentState(GameState::MAIN_MENU), running(true) {}

void Game::run() {
    std::cout << "  Welcome to the Game prod. by svt3d  " << std::endl;
    InputHandler::showHelp();
    
    while (running) {
        try {
            switch (currentState) {
                case GameState::MAIN_MENU:
                    showMainMenu();
                    handleMainMenuInput();
                    break;
                    
                case GameState::PLAYING:
                    gameSession.run(*this);
                    break;
                    
                case GameState::LEVEL_TRANSITION:
                    progressionSystem.handleLevelTransition(*this);
                    break;
                    
                case GameState::GAME_OVER:
                    showGameOverMenu();
                    break;
                    
                case GameState::VICTORY:
                    showVictoryScreen();
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            currentState = GameState::MAIN_MENU;
            cleanup();
        }
    }
}

void Game::showMainMenu() {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    if (SaveSystem::saveExists()) {
        std::cout << "2. Load Game" << std::endl;
    }
    std::cout << "3. Exit" << std::endl;
    std::cout << "Choose an option (1-3): ";
}

void Game::handleMainMenuInput() {
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case '1':
            startNewGame();
            break;
            
        case '2':
            loadGame();
            break;
            
        case '3':
            running = false;
            break;
            
        default:
            std::cout << "Invalid choice!" << std::endl;
            break;
    }
}

void Game::startNewGame() {
    gameSession.startNewGame();
    currentState = GameState::PLAYING;
}

void Game::loadGame() {
    if (!SaveSystem::saveExists()) {
        std::cout << "\n=== NO SAVE GAME FOUND ===" << std::endl;
        std::cout << "No save file exists. Starting new game instead..." << std::endl;
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        
        startNewGame();
        return;
    }
    
    try {
        std::cout << "\n=== LOADING GAME ===" << std::endl;
        std::cout << "Reading save file..." << std::endl;
        
        auto [loadedController, loadedLevelManager] = SaveSystem::loadGame();
        
        std::cout << "Save file read successfully!" << std::endl;
        std::cout << "Initializing game session..." << std::endl;
        
        gameSession.setController(std::move(loadedController));
        gameSession.setLevelManager(std::move(loadedLevelManager));
        
        std::cout << "=== GAME LOADED SUCCESSFULLY ===" << std::endl;
        
        if (loadedLevelManager) {
            std::cout << "Level: " << loadedLevelManager->getCurrentLevel() << std::endl;
        }
        
        currentState = GameState::PLAYING;
        std::cout << "\nPress Enter to start playing...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        
    } catch (const LoadException& e) {
        std::cerr << "\n=== LOAD FAILED ===" << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "Starting new game instead..." << std::endl;
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        
        startNewGame();
    } catch (const std::exception& e) {
        std::cerr << "\n=== UNEXPECTED ERROR ===" << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "Starting new game instead..." << std::endl;
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        
        startNewGame();
    }
}

void Game::setState(GameState newState) {
    currentState = newState;
}

void Game::stop() {
    running = false;
}

void Game::cleanup() {
    gameSession.cleanup();
}

void Game::showGameOverMenu() {
    std::cout << "\n=== GAME OVER ===" << std::endl;
    if (gameSession.getController()) {
        std::cout << "Final Score: " << gameSession.getController()->getPlayer().getScore() << std::endl;
    }
    
    std::cout << "1. Play Again" << std::endl;
    std::cout << "2. Main Menu" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Choose an option (1-3): ";
    
    handleGameOverInput();
}

void Game::showVictoryScreen() {
    std::cout << "\n*** VICTORY! ***" << std::endl;
    std::cout << "Congratulations! You completed all levels!" << std::endl;
    
    if (gameSession.getController()) {
        std::cout << "Final Score: " << gameSession.getController()->getPlayer().getScore() << std::endl;
    }
    
    std::cout << "\n1. Main Menu" << std::endl;
    std::cout << "2. Exit" << std::endl;
    std::cout << "Choose an option (1-2): ";
    
    handleVictoryInput();
}

void Game::handleGameOverInput() {
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case '1':
            startNewGame();
            break;
        case '2':
            currentState = GameState::MAIN_MENU;
            cleanup();
            break;
        case '3':
            running = false;
            break;
        default:
            currentState = GameState::MAIN_MENU;
            cleanup();
            break;
    }
}

void Game::handleVictoryInput() {
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case '1':
            currentState = GameState::MAIN_MENU;
            cleanup();
            break;
        case '2':
            running = false;
            break;
        default:
            currentState = GameState::MAIN_MENU;
            cleanup();
            break;
    }
}