#include "GameManager.h"
#include <iostream>

GameManager::GameManager()
    : gameLogic(gameState, actionProcessor, shopSystem),
    controller(gameState, actionProcessor, shopSystem, saveSystem),
    visualizer() {
}

void GameManager::run() {
    showMainMenu();
}

void GameManager::showMainMenu() {
    int choice;
    do {
        std::cout << "=== MAIN MENU ===" << std::endl;
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2:
            loadGame();
            break;
        case 3:
            std::cout << "Goodbye!" << std::endl;
            return;
        default:
            std::cout << "Invalid option!" << std::endl;
        }
    } while (choice != 3);
}

void GameManager::startNewGame() {
    gameState.initializeNewGame();
    gameLogic.reset();
    gameLogic.startGame();
    std::cout << "New game started!" << std::endl;
    runGameLoop();
}

void GameManager::loadGame() {
    try {
        if (saveSystem.loadGame(gameState)) {
            gameLogic.reset();
            gameLogic.startGame();
            std::cout << "Game loaded successfully!" << std::endl;
            runGameLoop();
        }
    }
    catch (const SaveLoadException& e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << "Starting new game instead..." << std::endl;
        startNewGame();
    }
}

void GameManager::runGameLoop() {
    visualizer.showMessage("Game Started! Use WASD to move, C to cast spells, M for shop, P to save, Q to quit");
    visualizer.render(gameState);

    while (controller.isGameRunning() && gameLogic.isGameRunning()) {
        try {
            if (gameLogic.checkPlayerVictory()) {
                handleVictory();
                break;
            }

            visualizer.showMessage("=== YOUR TURN ===");
            controller.processInput();

            visualizer.showMessage("=== ENEMY TURN ===");
            actionProcessor.processEnemyMoves(gameState);

            if (!gameLogic.isPlayerAlive()) {
                handleGameOver();
                break;
            }

            visualizer.render(gameState);
        }
        catch (const std::exception& e) {
            std::cout << "Error in game loop: " << e.what() << std::endl;
        }
    }
}

void GameManager::handleGameOver() {
    visualizer.showMessage("GAME OVER! You were defeated!");
    visualizer.showMessage("Final score: " + std::to_string(gameLogic.getPlayerScore()));
    showMainMenu();
}

void GameManager::handleVictory() {
    visualizer.showMessage("VICTORY! All enemies defeated!");
    visualizer.showMessage("Final score: " + std::to_string(gameLogic.getPlayerScore()));
    showMainMenu();
}