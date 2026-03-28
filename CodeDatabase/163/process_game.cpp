#include "process_game.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cctype>

ProcessGame::ProcessGame() 
    : isGameActive(false), isPaused(false) {
    gameController = std::make_unique<GameController>();
}

char ProcessGame::getCharacter() const {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void ProcessGame::clearScreen() const {
    std::cout << "\033[2J\033[1;1H";
}

void ProcessGame::showPauseMenu() {
    clearScreen();
    std::cout << "=== ПАУЗА ===\n\n";
    std::cout << "1. Продолжить игру\n";
    std::cout << "2. Сохранить игру\n";
    std::cout << "3. Загрузить игру\n";
    std::cout << "4. Главное меню\n";
    std::cout << "0. Выйти из игры\n\n";
    std::cout << "Выберите опцию: ";
}

void ProcessGame::processInGameMenu() {
    bool inMenu = true;
    
    while (inMenu && isGameActive) {
        showPauseMenu();
        char choice = getCharacter();
        std::cout << choice << "\n";
        
        switch (choice) {
            case '1': // Продолжить
                inMenu = false;
                isPaused = false;
                break;
                
            case '2': // Сохранить
                gameController->saveGame();
                std::cout << "\nНажмите любую клавишу чтобы продолжить...\n";
                getCharacter();
                break;
                
            case '3': // Загрузить
                {
                    std::cout << "Введите имя файла сохранения: ";
                    std::string filename;
                    std::cin >> filename;
                    std::cin.ignore(1000, '\n');
                    
                    if (gameController->loadGame(filename)) {
                        std::cout << "Игра загружена успешно!\n";
                        std::cout << "Нажмите любую клавишу чтобы продолжить...\n";
                        getCharacter();
                        inMenu = false;
                        isPaused = false;
                        return;
                    } else {
                        std::cout << "Не удалось загрузить игру.\n";
                        std::cout << "Нажмите любую клавишу чтобы продолжить...\n";
                        getCharacter();
                    }
                }
                break;
                
            case '4': // Главное меню
                isGameActive = false;
                inMenu = false;
                break;
                
            case '0': // Выйти
                endGame();
                inMenu = false;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
                usleep(1000000);
                break;
        }
    }
}

bool ProcessGame::loadGame(const std::string& filename) {
    if (gameController->loadGame(filename)) {
        isGameActive = true;
        isPaused = false;
        return true;
    }
    return false;
}

void ProcessGame::initializeNewGame() {
    gameController = std::make_unique<GameController>();
    gameController->initializeNewGame();
    isGameActive = true;
    isPaused = false;
}

void ProcessGame::startGameLoop() {
    if (!gameController || !isGameActive) return;
    
    while (isGameActive && !gameController->isGameOver() && 
           !gameController->isLevelComplete()) {
        
        if (!isPaused) {
            if (!gameController->isGameActive()) {
                endGame();
                break;
            }
            
            gameController->processPlayerTurn();
            
            if (!gameController->isPlayerTurnCompleted()) {
                processInGameMenu();
                if (!isGameActive) break;
                continue;
            }
            
            if (gameController->isGameOver() || 
                gameController->isLevelComplete()) {
                break;
            }
            
            gameController->processEnemyTurn();
            
            gameController->nextTurn();
            
        } else {
            usleep(100000);
        }
    }
    
    if (gameController->isGameOver()) {
        clearScreen();
        std::cout << "=== ИГРА ОКОНЧЕНА ===\n\n";
        std::cout << "Вы были побеждены!\n";
        std::cout << "Уровень: " << gameController->getCurrentLevel() << "\n";
        std::cout << "Очки: " << gameController->getScore() << "\n\n";
        
        std::cout << "1. Попробовать снова\n";
        std::cout << "2. Главное меню\n";
        std::cout << "0. Выйти\n\n";
        std::cout << "Выберите опцию: ";
        
        char choice = getCharacter();
        std::cout << choice << "\n";
        
        switch (choice) {
            case '1':
                initializeNewGame();
                startGameLoop();
                break;
            case '2':
                isGameActive = false;
                break;
            default:
                endGame();
                break;
        }
    } else if (gameController->isLevelComplete()) {
        clearScreen();
        std::cout << "=== УРОВЕНЬ ПРОЙДЕН! ===\n\n";
        std::cout << "Поздравляем! Вы прошли уровень " 
                  << gameController->getCurrentLevel() << "!\n";
        std::cout << "Ходов потрачено: " << gameController->getCurrentTurn() << "\n";
        std::cout << "Очков набрано: " << gameController->getScore() << "\n\n";
        
        std::cout << "1. Следующий уровень\n";
        std::cout << "2. Главное меню\n";
        std::cout << "0. Выйти\n\n";
        std::cout << "Выберите опцию: ";
        
        char choice = getCharacter();
        std::cout << choice << "\n";
        
        switch (choice) {
            case '1':
                initializeNewGame();
                startGameLoop();
                break;
            case '2':
                isGameActive = false;
                break;
            default:
                endGame();
                break;
        }
    }
}

void ProcessGame::pauseGame() {
    isPaused = true;
}

void ProcessGame::resumeGame() {
    isPaused = false;
}

void ProcessGame::endGame() {
    isGameActive = false;
    isPaused = false;
}

bool ProcessGame::isRunning() const {
    return isGameActive;
}

bool ProcessGame::isGamePaused() const {
    return isPaused;
}