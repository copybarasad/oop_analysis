#include "GameController.h"
#include "SaveManager.h"
#include "ControlConfig.h"
#include "GameLoop.h"
#include "ConsoleInputReader.h"
#include "ConsoleGameRenderer.h"
#include "GameEventObserver.h"
#include "FileLogger.h"
#include "ConsoleLogger.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <memory>

namespace fs = std::filesystem;

GameController::GameController() : gamePtr(nullptr), controlConfig_(nullptr), eventObserver_(nullptr) {}

GameController::~GameController() {
    if (gamePtr != nullptr) {
        delete gamePtr;
    }
    if (controlConfig_ != nullptr) {
        delete controlConfig_;
    }
    if (eventObserver_ != nullptr) {
        delete eventObserver_;
    }
}

void GameController::setControlsFile(const std::string& filename) {
    if (controlConfig_ == nullptr) {
        controlConfig_ = new ControlConfig();
    }
    bool loaded = controlConfig_->loadFromFile(filename);
    if (!loaded) {
        controlConfig_->setDefaultBindings();
    }
    if (gamePtr != nullptr) {
        gamePtr->setControlConfig(controlConfig_);
    }
}

void GameController::start() {
    showMainMenu();
}

void GameController::showMainMenu() {
    bool exitProgram = false;
    while (!exitProgram) {
        std::cout << "=== Игра (ЛР3) ===\n";
        std::cout << "1) Новая игра\n2) Загрузить игру\n3) Выйти из программы\nВыберите: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string tmp;
            std::getline(std::cin, tmp);
            std::cout << "Неверный ввод. Повторите.\n";
            continue;
        }
        std::string dummy;
        std::getline(std::cin, dummy);

        if (choice == 3) {
            std::cout << "Выход из программы.\n";
            exitProgram = true;
        }
        else if (choice == 2) {
            displaySaveFiles();
        }
        else if (choice == 1) {
            startNewGame();
        }
        else {
            std::cout << "Неверный выбор. Возврат в меню.\n";
        }
    }
}

void GameController::displaySaveFiles() {
    auto files = listSaveFiles(".");
    if (files.empty()) {
        std::cout << "Сохранений не найдено.\n";
        startNewGame();
    }
    else {
        std::cout << "Доступные сохранения:\n";
        for (size_t i = 0; i < files.size(); ++i) {
            std::cout << "  [" << i << "] " << files[i] << "\n";
        }
        std::cout << "Выберите номер файла для загрузки: ";
        std::string input;
        std::getline(std::cin, input);
        try {
            int idx = std::stoi(input);
            if (idx >= 0 && idx < (int)files.size()) {
                loadGame(files[idx]);
            }
        }
        catch (...) {
            std::cout << "Неверный номер файла.\n";
        }
    }
}

std::vector<std::string> GameController::listSaveFiles(const std::string& dir) {
    std::vector<std::string> names;
    try {
        for (auto& p : fs::directory_iterator(dir)) {
            if (!p.is_regular_file()) continue;
            std::string ext = p.path().extension().string();
            if (ext == ".sav" || ext == ".txt") {
                names.push_back(p.path().filename().string());
            }
        }
    }
    catch (...) {
    }
    std::sort(names.begin(), names.end());
    return names;
}

void GameController::startNewGame() {
    if (gamePtr != nullptr) {
        delete gamePtr;
    }
    gamePtr = new Game(12, 12, 6);  
    if (controlConfig_ != nullptr) {
        gamePtr->setControlConfig(controlConfig_);
    }
    if (eventObserver_ != nullptr) {
        gamePtr->setEventObserver(eventObserver_);
    }
    
    ConsoleInputReader reader(controlConfig_);
    ConsoleGameRenderer renderer;
    GameLoop<ConsoleInputReader, ConsoleGameRenderer> loop(gamePtr, std::move(reader), std::move(renderer), eventObserver_);
    loop.run();
}

bool GameController::loadGame(const std::string& filename) {
    if (gamePtr != nullptr) {
        delete gamePtr;
    }
    gamePtr = new Game(12, 12, 6);  
    if (controlConfig_ != nullptr) {
        gamePtr->setControlConfig(controlConfig_);
    }
    if (eventObserver_ != nullptr) {
        gamePtr->setEventObserver(eventObserver_);
    }
    try {
        gamePtr->loadFromFile(filename);
        
        ConsoleInputReader reader(controlConfig_);
        ConsoleGameRenderer renderer;
        GameLoop<ConsoleInputReader, ConsoleGameRenderer> loop(gamePtr, std::move(reader), std::move(renderer), eventObserver_);
        loop.run();
    }
    catch (const std::exception& e) {
        std::cout << "Ошибка загрузки: " << e.what() << "\n";
        return false;
    }
    return true;
}

void GameController::saveGame(const std::string& filename) {
    if (gamePtr != nullptr) {
        try {
            gamePtr->saveToFile(filename);
            std::cout << "Игра сохранена в файл " << filename << "\n";
            if (eventObserver_ != nullptr) {
                eventObserver_->onGameSaved(filename);
            }
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка сохранения: " << e.what() << "\n";
        }
    }
}

void GameController::setLoggingMode(const std::string& mode) {
    if (eventObserver_ != nullptr) {
        delete eventObserver_;
        eventObserver_ = nullptr;
    }
    
    if (mode == "file") {
        eventObserver_ = new GameEventObserver();
        FileLogger* fileLogger = new FileLogger("game.log");
        eventObserver_->addLogger(fileLogger);
    }
    else if (mode == "console") {
        eventObserver_ = new GameEventObserver();
        ConsoleLogger* consoleLogger = new ConsoleLogger();
        eventObserver_->addLogger(consoleLogger);
    }
    
    if (gamePtr != nullptr && eventObserver_ != nullptr) {
        gamePtr->setEventObserver(eventObserver_);
    }
}
