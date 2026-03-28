#include "Game.h"
#include "SaveLoadException.h"
#include "../models/events/EventNotifier.h"
#include "../models/events/PrintMessageEvent.h"
#include <iostream>
#include <limits>
#include <stdexcept>

Game::Game()
    : levelManager(std::make_unique<LevelManager>()),
      saveLoadManager(std::make_unique<SaveLoadManager>()),
      spellFactory(std::make_unique<SpellFactory>())
{
    // Контроллеры будут созданы после инициализации уровня
}

void Game::initializeControllers() {
    auto& field = levelManager->getField();
    
    playerController = std::make_unique<PlayerController>(&field);
    enemyController = std::make_unique<EnemyController>(&field);
    petController = std::make_unique<PetController>(&field);
    buildingController = std::make_unique<EnemyBuildingController>(&field);
    handController = std::make_unique<HandController>(&field, spellFactory.get());
    
    // Устанавливаем текущий уровень для контроллеров врагов
    enemyController->setCurrentLevel(levelManager->getCurrentLevel());
    buildingController->setCurrentLevel(levelManager->getCurrentLevel());
}

void Game::handleStartChoice() {
    char choice;
    bool validChoice = false;
    
    while (!validChoice) {
        PrintMessageEvent promptEvent("Начать новую игру (n) или загрузить сохранение (l)? ");
        EventNotifier::getInstance().notify(promptEvent);
        
        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            PrintMessageEvent errorEvent("Некорректный ввод. Введите 'n' для новой игры или 'l' для загрузки.");
            EventNotifier::getInstance().notify(errorEvent);
            continue;
        }
        
        if (choice == 'l' || choice == 'L') {
            try {
                std::string filename;
                PrintMessageEvent filenameEvent("Введите имя файла для загрузки (без расширения): ");
                EventNotifier::getInstance().notify(filenameEvent);
                std::cin >> filename;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                loadGameFromFile(filename);
                validChoice = true;
            } catch (const std::exception& e) {
                PrintMessageEvent errorEvent("Ошибка при загрузке игры: " + std::string(e.what()));
                EventNotifier::getInstance().notify(errorEvent);
                PrintMessageEvent newGameEvent("Начинаем новую игру...");
                EventNotifier::getInstance().notify(newGameEvent);
                startNewGame();
                validChoice = true;
            }
        } else if (choice == 'n' || choice == 'N') {
            startNewGame();
            validChoice = true;
        } else {
            PrintMessageEvent invalidEvent("Некорректный выбор. Введите 'n' для новой игры или 'l' для загрузки.");
            EventNotifier::getInstance().notify(invalidEvent);
        }
    }
}

void Game::startGame() {
    PrintMessageEvent welcomeEvent("=== ДОБРО ПОЖАЛОВАТЬ В ИГРУ ===");
    EventNotifier::getInstance().notify(welcomeEvent);
    PrintMessageEvent instructionEvent("Управляйте игроком (P) и избегайте врагов (E)!");
    EventNotifier::getInstance().notify(instructionEvent);
    PrintMessageEvent commandsEvent("Команды: z - сохранить, l - загрузить");
    EventNotifier::getInstance().notify(commandsEvent);
    
    handleStartChoice();
}

void Game::startNewGame() {
    levelManager->initializeLevel(1);
    initializeControllers();
    // Генерируем врагов и здания после пересоздания контроллеров
    enemyController->generateRandomEnemies();
    buildingController->generateRandomBuildings();
}

void Game::loadGameFromFile(const std::string& filename) {
    if (filename.empty()) {
        throw LoadException("Имя файла не может быть пустым");
    }
    
    int level = 1;
    auto field = std::make_unique<GameField>(15, 15);
    saveLoadManager->loadGame(filename + ".json", level, *field);
    
    levelManager->setCurrentLevel(level);
    levelManager->setField(std::move(field));
    
    // Пересоздаем контроллеры с новым полем
    initializeControllers();
    
    PrintMessageEvent loadSuccessEvent("Игра успешно загружена из файла: " + filename + ".json");
    EventNotifier::getInstance().notify(loadSuccessEvent);
}

void Game::saveCurrentGame(const std::string& filename) {
    try {
        std::string fullFilename = filename;
        if (fullFilename.find(".json") == std::string::npos) {
            fullFilename += ".json";
        }
        saveLoadManager->saveGame(fullFilename, levelManager->getCurrentLevel(), levelManager->getField());
        PrintMessageEvent saveSuccessEvent("Игра успешно сохранена в файл: " + fullFilename);
        EventNotifier::getInstance().notify(saveSuccessEvent);
    } catch (const SaveLoadException& e) {
        PrintMessageEvent errorEvent(e.what());
        EventNotifier::getInstance().notify(errorEvent);
    }
}

void Game::restartGame() {
    levelManager->setCurrentLevel(1);
    levelManager->initializeLevel(1);
    initializeControllers();
    enemyController->generateRandomEnemies();
    buildingController->generateRandomBuildings();
}

void Game::nextLevel(bool upgradeHealth, bool upgradeDamage) {
    levelManager->nextLevel(upgradeHealth, upgradeDamage);
    std::cout << levelManager->getField().getWidth() << std::endl;
    initializeControllers();
    enemyController->generateRandomEnemies();
    buildingController->generateRandomBuildings();
}
