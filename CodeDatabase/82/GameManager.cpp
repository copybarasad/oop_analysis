#include "GameManager.h"
#include "SaveSystem.h"
#include "GameException.h"
#include <iostream>
#include <limits>
#include <fstream>

GameManager::GameManager() : isRunning(true) {}

void GameManager::run() {
    promptForNewGameOrLoad();
    if (gameState) {
        runGameLoop();
    }
}

void GameManager::promptForNewGameOrLoad() {
    while (!gameState && isRunning) {
        std::cout << "\n=== ДОБРО ПОЖАЛОВАТЬ В ПОДЗЕМЕЛЬЯ ===" << std::endl;
        std::cout << "1. Новая игра" << std::endl;
        std::cout << "2. Загрузить игру" << std::endl;
        std::cout << "3. Выйти" << std::endl;
        std::cout << "Выберите действие (1-3): ";

        int choice;
        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                startNewGame();
                break;
            case 2:
                loadGame();
                break;
            case 3:
                isRunning = false;
                break;
            default:
                std::cout << "Неверный выбор. Пожалуйста, введите 1, 2 или 3." << std::endl;
        }
    }
}

void GameManager::startNewGame() {
    std::cout << "\n=== НАЧАЛО НОВОЙ ИГРЫ ===" << std::endl;
    int width, height;
    while (true) {
        std::cout << "Введите ширину поля (10-25): ";
        std::cin >> width;
        if (std::cin.fail() || width < 10 || width > 25) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите число от 10 до 25." << std::endl;
        } else {
            break;
        }
    }
    while (true) {
        std::cout << "Введите высоту поля (10-25): ";
        std::cin >> height;
        if (std::cin.fail() || height < 10 || height > 25) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите число от 10 до 25." << std::endl;
        } else {
            break;
        }
    }
    try {
        gameState = GameState::createNewGame(width, height);
        std::cout << "Новая игра начата!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при создании новой игры: " << e.what() << std::endl;
    }
}

void GameManager::loadGame() {
    std::string filename;
    std::cout << "Введите имя файла для загрузки: ";
    std::cin >> filename;
    try {
        gameState = SaveSystem::loadGame(filename);
        std::cout << "Игра успешно загружена!" << std::endl;
    } catch (const SaveLoadException& e) {
        std::cerr << "Ошибка загрузки: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Неизвестная ошибка при загрузке: " << e.what() << std::endl;
    }
}

void GameManager::runGameLoop() {
    while (gameState->getIsGameRunning() && isRunning) {
        #ifdef _WIN32
        int result = system("cls");
        #else
        int result = system("clear");
        #endif
        if (result != 0) {
            std::cerr << "Ошибка при очистке экрана" << std::endl;
        }

        renderer.renderGameState(*gameState);

        handlePlayerTurn();
        if (!gameState->getIsGameRunning()) continue; 

        checkGameOver();
    }
    renderer.renderGameOverScreen(*gameState);
    gameState.reset(); 
}

void GameManager::handlePlayerTurn() {
    GameAction action = inputHandler.getNextAction(); 

    if (action == GameAction::SAVE_GAME) { 
        saveGame();
        return;
    }

    if (action == GameAction::QUIT_GAME) {
        isRunning = false;
        return;
    }

    bool actionProcessed = gameLogic.processPlayerAction(*gameState, action);
    if (actionProcessed && gameState->getIsGameRunning()) {
        gameLogic.updateEnemies(*gameState); 
    }
}

void GameManager::checkGameOver() {
    if (gameLogic.checkGameOver(*gameState)) {
        gameState->setGameRunning(false);
    }
}

void GameManager::saveGame() {
    std::string filename;
    std::cout << "Введите имя файла для сохранения: ";
    std::cin >> filename;
    
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    try {
        SaveSystem::saveGame(*gameState, filename);
        std::cout << "Игра успешно сохранена в файл: " << filename << std::endl;
        std::cout << "Нажмите Enter для продолжения...";
        std::cin.get(); 
    } catch (const SaveLoadException& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << std::endl;
        std::cout << "Нажмите Enter для продолжения...";
        std::cin.get(); 
    } catch (const std::exception& e) {
        std::cerr << "Неизвестная ошибка при сохранении: " << e.what() << std::endl;
        std::cout << "Нажмите Enter для продолжения...";
        std::cin.get(); 
    }
}