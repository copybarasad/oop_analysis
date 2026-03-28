#define NOMINMAX
#include "GameManager.hpp"
#include "GameInterface.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>

GameManager::GameManager(GameInterface* gameInterface) 
    : currentState(std::make_unique<GameState>(1)),
      gameInterface(gameInterface), 
      currentPhase(GamePhase::MainMenu), 
      gameRunning(false),
      saveLoadManager("game_save.dat"),
      inputHandler(gameInterface) {
    
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
        
        CONSOLE_FONT_INFOEX fontInfo;
        fontInfo.cbSize = sizeof(fontInfo);
        GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
        wcscpy(fontInfo.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &fontInfo);
    #endif
}

void GameManager::startGame() {
    gameRunning = true;
    gameLoop();
}

void GameManager::gameLoop() {
    while (gameRunning) {
        switch (currentPhase) {
            case GamePhase::MainMenu:
                processMainMenu();
                break;
            case GamePhase::PlayerTurn:
                processPlayerTurn();
                break;
            case GamePhase::AlliesTurn:
                processAlliesTurn();
                break;
            case GamePhase::EnemiesTurn:
                processEnemiesTurn();
                break;
            case GamePhase::BuildingsTurn:
                processBuildingsTurn();
                break;
            case GamePhase::LevelTransition:
                processLevelTransition();
                break;
            case GamePhase::UpgradePhase:
                processUpgradePhase();
                break;
            case GamePhase::GameOver:
                processGameOver();
                break;
            case GamePhase::Victory:
                processVictory();
                break;
            case GamePhase::SaveGame:
                saveGame();
                currentPhase = GamePhase::PlayerTurn;
                break;
            case GamePhase::LoadGame:
                loadGame();
                currentPhase = GamePhase::PlayerTurn;
                break;
        }
    }
}

void GameManager::processMainMenu() {
    gameInterface->showMainMenu();
    
    char choice;
    std::cin >> choice;
    
    switch (choice) {
        case '1':
            currentState = std::make_unique<GameState>(1);
            currentPhase = GamePhase::PlayerTurn;
            break;
        case '2':
            currentPhase = GamePhase::LoadGame;
            break;
        case '3':
            gameRunning = false;
            break;
        default:
            gameInterface->showMessage("Неверный выбор!");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            break;
    }
}

void GameManager::processPlayerTurn() {
    bool actionPerformed = false;
    while (!actionPerformed && gameRunning) {
        gameInterface->showGameState();
        
        bool inputProcessed = inputHandler.handleMovement(*currentState, currentPhase, gameRunning);
        
        if (currentPhase == GamePhase::SaveGame || currentPhase == GamePhase::LoadGame) {
            return;
        }
        
        if (inputProcessed) {
            actionPerformed = true;
        }
    }
    
    if (isLevelComplete()) {
        currentPhase = GamePhase::LevelTransition;
    }
    if (isGameOver()) {
        currentPhase = GamePhase::GameOver;
    } else {
        currentPhase = GamePhase::AlliesTurn;
    }
}

void GameManager::processAlliesTurn() {
    EntityManager::updateAllies(*currentState);
    currentPhase = GamePhase::EnemiesTurn;
}

void GameManager::processEnemiesTurn() {
    EntityManager::updateEnemies(*currentState);
    currentPhase = GamePhase::BuildingsTurn;
}

void GameManager::processBuildingsTurn() {
    EntityManager::updateBuildings(*currentState);
    EntityManager::updateTraps(*currentState);
    EntityManager::syncFieldWithEntities(*currentState);
    currentPhase = GamePhase::PlayerTurn;
}

void GameManager::processLevelTransition() {
    SpellHand& hand = currentState->player.getSpellHand();
    int spellsToRemove = hand.getSpellCount() / 2;
    
    for (int i = 0; i < spellsToRemove; ++i) {
        if (hand.getSpellCount() > 0) {
            hand.removeSpell(rand() % hand.getSpellCount());
        }
    }
    
    gameInterface->showLevelTransition();
    currentPhase = GamePhase::UpgradePhase;
}

void GameManager::processUpgradePhase() {
    gameInterface->showUpgradePhase();
    
    int nextLevel = currentState->currentLevel + 1;
    currentState->resetForNewLevel(nextLevel);
    currentPhase = GamePhase::PlayerTurn;
}

void GameManager::processGameOver() {
    gameInterface->showGameOver();
    
    char choice;
    std::cin >> choice;
    
    switch (choice) {
        case '1':
            currentState = std::make_unique<GameState>(1);
            currentPhase = GamePhase::PlayerTurn;
            break;
        case '2':
            currentPhase = GamePhase::MainMenu;
            break;
        case '3':
            gameRunning = false;
            break;
        default:
            currentPhase = GamePhase::MainMenu;
            break;
    }
}

void GameManager::processVictory() {
    gameInterface->showVictory();
    
    char choice;
    std::cin >> choice;
    
    switch (choice) {
        case '1':
            currentState = std::make_unique<GameState>(1);
            currentPhase = GamePhase::PlayerTurn;
            break;
        case '2':
            gameRunning = false;
            break;
        default:
            gameRunning = false;
            break;
    }
}

void GameManager::saveGame() {
    try {
        saveLoadManager.saveGame(*currentState);
        gameInterface->showMessage("Игра успешно сохранена!");
    }
    catch (const std::exception& e) {
        gameInterface->showMessage("Ошибка сохранения: " + std::string(e.what()));
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void GameManager::loadGame() {
    try {
        auto loadedState = saveLoadManager.loadGame();
        
        if (loadedState) {
            currentState = std::move(loadedState);
            gameInterface->showMessage("Игра успешно загружена!");
        } else {
            throw GameLoadException("Не удалось загрузить состояние игры");
        }
    }
    catch (const std::exception& e) {
        gameInterface->showMessage("Ошибка загрузки: " + std::string(e.what()));
        gameInterface->showMessage("Запускается новая игра...");
        currentState = std::make_unique<GameState>(1);
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

bool GameManager::isLevelComplete() const {
    return currentState->enemies.empty() && currentState->buildings.empty();
}

bool GameManager::isGameOver() const {
    return !currentState->player.isAlive();
}