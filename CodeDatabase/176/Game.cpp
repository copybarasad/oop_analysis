#include "Game.h"
#include <iostream>
#include <limits>

Game::Game() : gameRunning(true) {}

void Game::run() {
    while (gameRunning) {
        showMainMenu();
    }
    std::cout << "Thanks for playing!\n";
}

void Game::showMainMenu() {
    std::cout << "\n=== MAGIC BATTLE ===\n";
    std::cout << "1. Start New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose option: ";

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
            gameRunning = false;
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
    }
}

void Game::startNewGame() {
    auto controller = std::make_unique<GameController>();
    controller->startNewGame();

    TemplateGameManager<InputHandler, GameRenderer> gameManager(controller.get());
    gameManager.runGameLoop();
}

void Game::loadGame() {
    auto controller = std::make_unique<GameController>();
    
    if (controller->loadGame()) {
        TemplateGameManager<InputHandler, GameRenderer> gameManager(controller.get());
        gameManager.runGameLoop();
    } else {
        std::cout << "Failed to load game!\n";
    }
}
