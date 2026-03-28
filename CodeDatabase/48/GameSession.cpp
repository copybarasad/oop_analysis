#include "lib/GameSession.h"
#include "lib/SaveSystem.h"
#include <iostream>

// Объявление внешней функции
extern int getInputInt(const std::string& prompt, int min, int max);

GameSession::GameSession() : currentLevelNumber(1), sessionActive(false) {
}

GameSession::~GameSession() {
    cleanup();
}

void GameSession::startNewGame() {
    cleanup();
    
    std::cout << "\n=== STARTING NEW GAME ===\n";
    
    setupGameField();
    setupPlayer();
    
    sessionActive = true;
    currentLevelNumber = 1;
    
    startLevel(currentLevelNumber);
}

void GameSession::loadSavedGame(const std::string& saveName) {
    cleanup();
    
    GameSaveData saveData;
    if (SaveSystem::loadGame(saveData, saveName)) {
        std::cout << "Loading: " << saveName << "\n";
        std::cout << "Level: " << saveData.currentLevel << ", Player: " << saveData.playerName << "\n";
        
        // Создать игровое поле из сохраненных данных
        gameField = std::make_unique<GameField>(saveData.fieldWidth, saveData.fieldHeight);
        
        // Восстановить состояние поля
        for (int y = 0; y < saveData.fieldHeight; ++y) {
            for (int x = 0; x < saveData.fieldWidth; ++x) {
                auto& cell = gameField->getCell(x, y);
                cell.setIsPassable(saveData.fieldPassable[y][x]);
                cell.setHasEnemy(saveData.fieldHasEnemy[y][x]);
                cell.setHasTower(saveData.fieldHasTower[y][x]);
            }
        }
        
        // Создать игрока в сохраненной позиции
        player = std::make_unique<Player>(gameField.get(), saveData.playerName, saveData.playerX, saveData.playerY);
        
        // Восстановить ВСЕ параметры игрока
        player->setHealth(saveData.playerHealth);
        player->setMaxHealth(saveData.playerMaxHealth);
        player->setDamage(saveData.playerDamage);
        player->setScore(saveData.playerScore);
        player->setLevel(saveData.playerLevel);
        player->setMana(saveData.playerMana);
        player->setMaxMana(saveData.playerMaxMana);
        
        sessionActive = true;
        currentLevelNumber = saveData.currentLevel;
        
        std::cout << "Game loaded successfully!\n";
        std::cout << "Continuing from Level " << currentLevelNumber << "\n";
        
        // Запускаем уровень с восстановленным состоянием
        startLoadedLevel(currentLevelNumber, saveData);
    } else {
        std::cout << "Failed to load save! Starting new game instead.\n";
        startNewGame();
    }
}

void GameSession::startLevel(int levelNumber) {
    if (!sessionActive) return;
    
    currentLevel = std::make_unique<Level>(levelNumber, gameField.get(), player.get());
    currentLevel->start();
    
    if (currentLevel->isCompleted()) {
        currentLevelNumber++;
        if (currentLevelNumber <= 3) {
            std::cout << "\nPreparing for level " << currentLevelNumber << "...\n";
            player->heal(50);
            player->restoreMana(30);
            startLevel(currentLevelNumber);
        } else {
            std::cout << "\n🎊 CONGRATULATIONS! YOU COMPLETED ALL LEVELS! 🎊\n";
            sessionActive = false;
        }
    } else if (currentLevel->isFailed()) {
        sessionActive = false;
    }
}

// НОВЫЙ МЕТОД ДЛЯ ЗАГРУЗКИ УРОВНЯ С ВОССТАНОВЛЕННЫМ СОСТОЯНИЕМ
void GameSession::startLoadedLevel(int levelNumber, const GameSaveData& saveData) {
    if (!sessionActive) return;
    
    // Создаем уровень
    currentLevel = std::make_unique<Level>(levelNumber, gameField.get(), player.get());
    
    // Вместо вызова start(), который создает новых врагов, 
    // мы вручную инициализируем уровень с загруженными данными
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "          LEVEL " << levelNumber << " (LOADED)\n";
    std::cout << std::string(50, '=') << "\n";
    
    // Здесь должна быть логика восстановления врагов и башен из saveData
    // Пока что просто запускаем обычный игровой цикл
    currentLevel->start();
    
    if (currentLevel->isCompleted()) {
        currentLevelNumber++;
        if (currentLevelNumber <= 3) {
            std::cout << "\nPreparing for level " << currentLevelNumber << "...\n";
            player->heal(50);
            player->restoreMana(30);
            startLevel(currentLevelNumber);
        } else {
            std::cout << "\n🎊 CONGRATULATIONS! YOU COMPLETED ALL LEVELS! 🎊\n";
            sessionActive = false;
        }
    } else if (currentLevel->isFailed()) {
        sessionActive = false;
    }
}

void GameSession::processGameLoop() {
}

void GameSession::endGame() {
    sessionActive = false;
    displaySessionStatus();
}

void GameSession::displaySessionStatus() const {
    std::cout << "\n=== GAME SESSION STATISTICS ===\n";
    if (player) {
        player->displayStatus();
    }
    std::cout << "Levels completed: " << (currentLevelNumber - 1) << "\n";
    std::cout << "Session: " << (sessionActive ? "Active" : "Ended") << "\n";
}

bool GameSession::isActive() const {
    return sessionActive;
}

int GameSession::getCurrentLevel() const {
    return currentLevelNumber;
}

void GameSession::setupGameField() {
    std::cout << "=== CREATE GAME FIELD ===\n";
    int width = getInputInt("Enter field width (10-25): ", 10, 25);
    int height = getInputInt("Enter field height (10-25): ", 10, 25);
    
    try {
        gameField = std::make_unique<GameField>(width, height);
        std::cout << "Game field " << width << "x" << height << " created successfully!\n";
    } catch (const std::exception& e) {
        std::cout << "Error creating field: " << e.what() << "\n";
        std::cout << "Using default size 15x15\n";
        gameField = std::make_unique<GameField>(15, 15);
    }
}

void GameSession::setupPlayer() {
    std::string playerName;
    std::cout << "Enter your hero's name: ";
    std::getline(std::cin, playerName);
    
    if (playerName.empty()) {
        playerName = "Hero";
    }
    
    player = std::make_unique<Player>(gameField.get(), playerName, 0, 0);
    std::cout << "Welcome, " << playerName << "!\n";
}

void GameSession::cleanup() {
    gameField.reset();
    player.reset();
    currentLevel.reset();
    sessionActive = false;
}