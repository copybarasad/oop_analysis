#include "Game.h"
#include "GameException.h"
#include "SaveGameException.h"
#include "LoadGameException.h"
#include "FileNotFoundException.h"
#include "CorruptedSaveException.h"
#include "EnemyTower.h"
#include "ConsoleInputHandler.h"  
#include "ConsoleRenderer.h" 
#include <iostream>
#include <sstream>
#include <limits>

Game::Game() : currentGame(nullptr), 
               saveManager(std::make_unique<SaveManager>()), 
               isRunning(false), 
               currentLevel(1), 
               justLoaded(false) {}

void Game::start() {
    isRunning = true;
    
    while (isRunning) {
        showMainMenu();
        processMainMenuInput();
    }
}

void Game::showMainMenu() {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

void Game::processMainMenuInput() {
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2: {
            std::string filename;
            std::cout << "Enter filename to load: ";
            std::getline(std::cin, filename);
            try {
                saveManager->loadGame(filename, currentGame.get(), currentLevel);
                justLoaded = true;
                
                gameController = std::make_unique<GameController<ConsoleInputHandler>>(currentGame.get());
                gameVisualizer = std::make_unique<GameVisualizer<ConsoleRenderer>>(currentGame.get());
                
                runLevel();
            } catch (const GameException& e) {
                std::cout << "Load error: " << e.what() << std::endl;
            }
            break;
        }
        case 3:
            isRunning = false;
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
    }
}

bool Game::processGameInput() {
    if (!gameController) return false;
    
    auto command = gameController->getNextCommand();
    
    if (!command) {
        return false;
    }
    
    switch (command->getType()) {
        case CommandType::SAVE_GAME: {
            std::string filename;
            std::cout << "Enter filename to save: ";
            std::getline(std::cin, filename);
            try {
                saveManager->saveGame(filename, currentGame.get(), currentLevel);
                std::cout << "Game saved!" << std::endl;
            } catch (const GameException& e) {
                std::cout << "Save error: " << e.what() << std::endl;
            }
            return false;
        }
        case CommandType::LOAD_GAME: {
            std::string filename;
            std::cout << "Enter filename to load: ";
            std::getline(std::cin, filename);
            try {
                saveManager->loadGame(filename, currentGame.get(), currentLevel);
                std::cout << "Game loaded!" << std::endl;
                justLoaded = true;
                
                gameController = std::make_unique<GameController<ConsoleInputHandler>>(currentGame.get());
                gameVisualizer = std::make_unique<GameVisualizer<ConsoleRenderer>>(currentGame.get());
            } catch (const GameException& e) {
                std::cout << "Load error: " << e.what() << std::endl;
            }
            return false;
        }
        case CommandType::QUIT_GAME:
            currentGame.reset();
            return false;
        default:
            return gameController->processGameCommand(command);
    }
}

void Game::runLevel() {
    while (currentGame && currentGame->isGameActive()) {
        if (gameVisualizer) {
            gameVisualizer->displayGame(currentLevel);
        }
        
        bool isPlayerTurn = playerTurn();
        if (!currentGame->isGameActive()) break;
        
        if (isPlayerTurn) {
            enemiesTurn();
        }
        
        if (!justLoaded) {
            if (checkLevelComplete()) {
                showLevelResult(true);
                askForRestart();
                break;
            } else if (checkGameOver()) {
                showLevelResult(false);
                askForRestart();
                break;
            }
        } else {
            justLoaded = false;
        }
    }
}

bool Game::playerTurn() {
    return processGameInput();
}


void Game::enemiesTurn() {
    int damage = currentGame->processEnemyTurns();
    if (damage > 0) {
        std::cout << "Enemies dealt " << damage << " damage!" << std::endl;
    }
}


bool Game::checkLevelComplete() {
    return currentGame->getAliveEnemyCount() == 0;
}

bool Game::checkGameOver() {
    return !currentGame->isGameActive();
}

void Game::showLevelResult(bool won) {
    if (won) {
        std::cout << "\n=== VICTORY! ===" << std::endl;
        std::cout << "You have defeated all enemies!" << std::endl;
    } else {
        std::cout << "\n=== DEFEAT ===" << std::endl;
        std::cout << "Your score: " << currentGame->getPlayerScore() << std::endl;
    }
}

void Game::startNewGame() {
    currentGame = std::make_unique<GameEngine>(10, 10);
    currentLevel = 1;
    justLoaded = false;
    
    gameController = std::make_unique<GameController<ConsoleInputHandler>>(currentGame.get());
    gameVisualizer = std::make_unique<GameVisualizer<ConsoleRenderer>>(currentGame.get());
    
    setupLevel();
    runLevel();
}

std::vector<Position> Game::getEnemyPositions() const {
    return currentGame ? currentGame->getEnemyPositions() : std::vector<Position>();
}

std::vector<int> Game::getEnemyHealths() const {
    return currentGame ? currentGame->getEnemyHealths() : std::vector<int>();
}

std::vector<int> Game::getEnemyDamages() const {
    return currentGame ? currentGame->getEnemyDamages() : std::vector<int>();
}

bool Game::isEnemyTower(int index) const {
    return currentGame ? currentGame->isEnemyTower(index) : false;
}

int Game::getEnemyAttackRange(int index) const {
    return currentGame ? currentGame->getEnemyAttackRange(index) : 0;
}

void Game::setupLevel() {
    currentGame->addEnemy(std::make_unique<Enemy>(30, 10), Position(2, 2));
    currentGame->addEnemy(std::make_unique<Enemy>(25, 8), Position(7, 7));
    currentGame->addEnemy(std::make_unique<EnemyTower>(50, 20, 4), Position(6, 7));
    
    std::cout << "Level " << currentLevel << " has started!" << std::endl;
    std::cout << "Destroy all enemies!" << std::endl;
}

void Game::askForRestart() {
    std::cout << "\nWould you like to restart? (y/n): ";
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "y" || choice == "Y") {
        startNewGame();
    } else {
        currentGame.reset();
        gameController.reset();
        gameVisualizer.reset();
    }
    
    justLoaded = false;
}