#include "Game.h"
#include "GameGrid.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <limits>

namespace fs = std::filesystem;

SaveFile::SaveFile(const std::string& filename, std::ios::openmode mode)
    : filename(filename) {
    file.open(filename, mode);
    if (!file.is_open()) {
        if (mode & std::ios::in) {
            throw FileNotFoundException(filename);
        } else {
            throw FilePermissionException(filename, "записать");
        }
    }
}

SaveFile::~SaveFile() {
    if (file.is_open()) {
        file.close();
    }
}

void SaveFile::close() {
    if (file.is_open()) {
        file.flush();
        file.close();
    }
}

SaveFile::SaveFile(SaveFile&& other) noexcept
    : file(std::move(other.file)), filename(std::move(other.filename)) {}

SaveFile& SaveFile::operator=(SaveFile&& other) noexcept {
    if (this != &other) {
        if (file.is_open()) {
            file.close();
        }
        file = std::move(other.file);
        filename = std::move(other.filename);
    }
    return *this;
}

Game::Game() 
    : currentLevelNumber(1), 
      totalScore(0), 
      isPaused(false),
      internalState(InternalState::MAIN_MENU),
      shouldExit(false) {}

void Game::run() {
    while (!shouldExit) {
        try {
            switch (internalState) {
                case InternalState::MAIN_MENU:
                    processMainMenu();
                    break;
                case InternalState::PLAYING:
                    processPlayingState();
                    break;
                case InternalState::LEVEL_COMPLETE:
                    processLevelComplete();
                    break;
                case InternalState::GAME_OVER:
                    processGameOver();
                    break;
            }
        } catch (const GameException& e) {
            std::cout << "Ошибка игры: " << e.what() << std::endl;
            if (internalState == InternalState::PLAYING) {
                std::cout << "Возвращаемся в игру..." << std::endl;
            } else {
                internalState = InternalState::MAIN_MENU;
            }
        }
    }
}

void Game::processMainMenu() {
    displayMainMenu();
    
    char choice;
    std::cout << "Выберите действие: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch (choice) {
        case '1': {
            int width, height;
            std::cout << "Введите размеры поля (ширина высота): ";
            std::cin >> width >> height;
            
            if (width < 10 || width > 25 || height < 10 || height > 25) {
                std::cout << "Неверные размеры! Должны быть от 10 до 25." << std::endl;
                return;
            }
            
            currentLevel = std::make_unique<GameGrid>(width, height);
            internalState = InternalState::PLAYING;
            std::cout << "Новая игра началась! Уровень " << currentLevelNumber << std::endl;
            break;
        }
        case '2':
            showLoadMenu();
            break;
        case '3':
            shouldExit = true;
            break;
        default:
            std::cout << "Неверный выбор!" << std::endl;
    }
}

void Game::showSaveMenu() {
    std::cout << "\n=== СОХРАНЕНИЕ ИГРЫ ===" << std::endl;
    std::cout << "Введите имя файла (без пробелов): ";
    std::string filename;
    std::cin >> filename;
    
    if (filename.find(".sav") == std::string::npos) {
        filename += ".sav";
    }
    
    try {
        SaveFile file(filename, std::ios::out | std::ios::binary);
        saveGameData(file);
        file.close();
        std::cout << "Игра сохранена в файл: " << filename << std::endl;
    } catch (const FilePermissionException& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        std::cout << "Попробуйте другое имя файла." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Ошибка при сохранении: " << e.what() << std::endl;
    }
}

void Game::showLoadMenu() {
    std::cout << "\n=== ЗАГРУЗКА ИГРЫ ===" << std::endl;
    
    auto saves = listSaves();
    if (saves.empty()) {
        std::cout << "Нет сохраненных игр." << std::endl;
        return;
    }
    
    std::cout << "Сохраненные игры:" << std::endl;
    for (size_t i = 0; i < saves.size(); i++) {
        std::cout << i + 1 << ". " << saves[i] << std::endl;
    }
    
    int choice;
    std::cout << "Выберите сохранение (0 - отмена): ";
    std::cin >> choice;
    
    if (choice > 0 && choice <= static_cast<int>(saves.size())) {
        std::string filename = saves[choice - 1];
        
        try {
            if (!validateSaveFile(filename)) {
                std::cout << "Файл сохранения поврежден!" << std::endl;
                return;
            }
            
            SaveFile file(filename, std::ios::in | std::ios::binary);
            loadGameData(file);
            file.close();
            
            internalState = InternalState::PLAYING;
            std::cout << "Игра загружена: " << filename << std::endl;
            std::cout << "Уровень: " << currentLevelNumber 
                      << ", Счет: " << totalScore << std::endl;
            
        } catch (const FileNotFoundException& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        } catch (const CorruptedSaveException& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Ошибка при загрузке: " << e.what() << std::endl;
        }
    }
}

void Game::processPlayingState() {
    if (isPaused) {
        std::cout << "Игра на паузе. Нажмите 'p' чтобы продолжить." << std::endl;
        return;
    }
    
    render();
    
    char input;
    std::cout << "Введите команду: ";
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    PlayerCommand command = parseInput(input);
    
    if (command == PlayerCommand::QUIT) {
        internalState = InternalState::MAIN_MENU;
        return;
    }
    
    if (command == PlayerCommand::NONE) {
        std::cout << "Неизвестная команда!" << std::endl;
        return;
    }
    
    executePlayerTurn(command);
    if (checkGameOver()) {
        internalState = InternalState::GAME_OVER;
        return;
    }
    
    executeAlliesTurn();
    executeEnemiesTurn();
    executeStructuresTurn();
    
    if (checkLevelCompletion()) {
        internalState = InternalState::LEVEL_COMPLETE;
    }
}

void Game::executePlayerTurn(PlayerCommand command) {
    if (!currentLevel) return;
    
    auto* player = currentLevel->getPlayer();
    if (!player) return;
    
    int currentX, currentY;
    currentLevel->getPlayerPosition(currentX, currentY);
    
    try {
        switch (command) {
            case PlayerCommand::MOVE_UP:
                currentLevel->movePlayer(currentX, currentY - 1);
                break;
            case PlayerCommand::MOVE_DOWN:
                currentLevel->movePlayer(currentX, currentY + 1);
                break;
            case PlayerCommand::MOVE_LEFT:
                currentLevel->movePlayer(currentX - 1, currentY);
                break;
            case PlayerCommand::MOVE_RIGHT:
                currentLevel->movePlayer(currentX + 1, currentY);
                break;
            case PlayerCommand::TOGGLE_MODE:
                player->toggleBattleMode();
                break;
            case PlayerCommand::CAST_SPELL: {
                player->displaySpells();
                if (player->getSpellHand().getSpellCount() > 0) {
                    std::cout << "Введите индекс заклинания и координаты цели (индекс x y): ";
                    int index, x, y;
                    if (std::cin >> index >> x >> y) {
                        player->castSpell(index, *currentLevel, x, y);
                    }
                }
                break;
            }
            case PlayerCommand::SAVE_GAME:
                showSaveMenu();
                break;
            case PlayerCommand::LOAD_GAME:
                showLoadMenu();
                break;
            case PlayerCommand::SHOW_HELP:
                displayHelp();
                break;
            case PlayerCommand::PAUSE:
                isPaused = !isPaused;
                std::cout << (isPaused ? "Игра на паузе" : "Игра продолжается") << std::endl;
                break;
            default:
                break;
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка во время хода игрока: " << e.what() << std::endl;
    }
}

void Game::executeEnemiesTurn() {
    if (!currentLevel) return;
    
    try {
        currentLevel->moveEnemies();
        currentLevel->removeDefeatedEnemies();
        
        auto* player = currentLevel->getPlayer();
        if (player && !player->isActive()) {
            throw GameStateException("Игрок побежден врагами");
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка во время хода врагов: " << e.what() << std::endl;
    }
}

void Game::executeStructuresTurn() {
    if (!currentLevel) return;
    
    try {
        currentLevel->generateEnemiesFromStructures();
    } catch (const std::exception& e) {
        std::cout << "Ошибка во время хода зданий: " << e.what() << std::endl;
    }
}

bool Game::checkLevelCompletion() const {
    if (!currentLevel) return false;
    return currentLevel->getEnemyCount() == 0;
}

bool Game::checkGameOver() const {
    if (!currentLevel) return true;
    auto* player = currentLevel->getPlayer();
    return !player || !player->isActive();
}

void Game::displayMainMenu() const {
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << std::endl;
    std::cout << "1. Новая игра" << std::endl;
    std::cout << "2. Загрузить игру" << std::endl;
    std::cout << "3. Выход" << std::endl;
}

void Game::displayHelp() const {
    std::cout << "\n=== СПРАВКА ===" << std::endl;
    std::cout << "Движение: W/A/S/D" << std::endl;
    std::cout << "Смена режима: F" << std::endl;
    std::cout << "Заклинания: C" << std::endl;
    std::cout << "Сохранить: S" << std::endl;
    std::cout << "Загрузить: L" << std::endl;
    std::cout << "Пауза: P" << std::endl;
    std::cout << "Справка: H" << std::endl;
    std::cout << "Выход в меню: Q" << std::endl;
}

void Game::displayGameState() const {
    if (!currentLevel) return;
    
    auto* player = currentLevel->getPlayer();
    if (!player) return;
    
    std::cout << "\n=== СОСТОЯНИЕ ИГРЫ ===" << std::endl;
    std::cout << "Уровень: " << currentLevelNumber << std::endl;
    std::cout << "Счет: " << totalScore << std::endl;
    std::cout << "Здоровье игрока: " << player->getHitPoints() << "/" 
              << player->getMaxHitPoints() << std::endl;
    std::cout << "Режим: " << (player->getCurrentMode() == BattleMode::CLOSE_COMBAT ? 
                             "Ближний бой" : "Дальний бой") << std::endl;
    std::cout << "Врагов осталось: " << currentLevel->getEnemyCount() << std::endl;
}

Game::PlayerCommand Game::parseInput(char input) const {
    switch (input) {
        case 'w':
        case 'W':
            return PlayerCommand::MOVE_UP;
        case 's':
        case 'S':
            return PlayerCommand::MOVE_DOWN;
        case 'a':
        case 'A':
            return PlayerCommand::MOVE_LEFT;
        case 'd':
        case 'D':
            return PlayerCommand::MOVE_RIGHT;
        case 'f':
        case 'F':
            return PlayerCommand::TOGGLE_MODE;
        case 'c':
        case 'C':
            return PlayerCommand::CAST_SPELL;
        case 's':
        case 'S':
            return PlayerCommand::SAVE_GAME;
        case 'l':
        case 'L':
            return PlayerCommand::LOAD_GAME;
        case 'h':
        case 'H':
            return PlayerCommand::SHOW_HELP;
        case 'p':
        case 'P':
            return PlayerCommand::PAUSE;
        case 'q':
        case 'Q':
            return PlayerCommand::QUIT;
        default:
            return PlayerCommand::NONE;
    }
}

void Game::processLevelComplete() {
    std::cout << "\n=== УРОВЕНЬ " << currentLevelNumber << " ПРОЙДЕН! ===" << std::endl;
    std::cout << "Набрано очков: " << totalScore << std::endl;
    std::cout << "Переходим на уровень " << (currentLevelNumber + 1) << "..." << std::endl;
    
    currentLevelNumber++;
    currentLevel = std::make_unique<GameGrid>(currentLevel->getWidth(), currentLevel->getHeight());
    internalState = InternalState::PLAYING;
}

void Game::processGameOver() {
    std::cout << "\n=== ИГРА ОКОНЧЕНА ===" << std::endl;
    std::cout << "Вы проиграли на уровне " << currentLevelNumber << std::endl;
    std::cout << "Финальный счет: " << totalScore << std::endl;
    
    std::cout << "\n1. Начать заново" << std::endl;
    std::cout << "2. Вернуться в главное меню" << std::endl;
    std::cout << "3. Выйти из игры" << std::endl;
    
    int choice;
    std::cout << "Выберите действие: ";
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            currentLevelNumber = 1;
            totalScore = 0;
            currentLevel = std::make_unique<GameGrid>(10, 10);
            internalState = InternalState::PLAYING;
            break;
        case 2:
            internalState = InternalState::MAIN_MENU;
            break;
        case 3:
            shouldExit = true;
            break;
        default:
            std::cout << "Неверный выбор!" << std::endl;
            internalState = InternalState::MAIN_MENU;
    }
}

void Game::saveGameData(SaveFile& file) const {
    auto& stream = file.get();
    
    stream << "GAME_SAVE_v1.0\n";
    stream << currentLevelNumber << "\n";
    stream << totalScore << "\n";
    
    if (currentLevel) {
        int playerX, playerY;
        currentLevel->getPlayerPosition(playerX, playerY);
        auto* player = currentLevel->getPlayer();
        
        stream << currentLevel->getWidth() << " " << currentLevel->getHeight() << "\n";
        stream << playerX << " " << playerY << "\n";
        stream << player->getHitPoints() << " "
               << player->getPoints() << " "
               << static_cast<int>(player->getCurrentMode()) << " "
               << player->getCurrentDamage() << "\n";
    }
}

void Game::loadGameData(SaveFile& file) {
    auto& stream = file.get();
    
    std::string header;
    std::getline(stream, header);
    
    if (header != "GAME_SAVE_v1.0") {
        throw CorruptedSaveException("Неверный заголовок файла");
    }
    
    stream >> currentLevelNumber;
    stream >> totalScore;
    
    int width, height;
    stream >> width >> height;
    
    currentLevel = std::make_unique<GameGrid>(width, height);
    
    int playerX, playerY;
    stream >> playerX >> playerY;
    
    int hp, points, mode, damage;
    stream >> hp >> points >> mode >> damage;
}

void Game::update() {
    if (!isPaused) {
        displayGameState();
    }
}

void Game::render() const {
    if (currentLevel) {
        for (int y = 0; y < currentLevel->getHeight(); y++) {
            for (int x = 0; x < currentLevel->getWidth(); x++) {
                if (currentLevel->hasPlayerAt(x, y)) {
                    std::cout << "P ";
                } else if (currentLevel->hasEnemyAt(x, y)) {
                    std::cout << "E ";
                } else if (currentLevel->hasStructureAt(x, y)) {
                    std::cout << "B ";
                } else {
                    switch (currentLevel->getCellTerrain(x, y)) {
                        case TerrainType::OBSTACLE: std::cout << "# "; break;
                        case TerrainType::SLOWING: std::cout << "~ "; break;
                        case TerrainType::CLEAR: std::cout << ". "; break;
                    }
                }
            }
            std::cout << std::endl;
        }
    }
}

bool Game::saveExists(const std::string& filename) {
    try {
        return fs::exists(filename);
    } catch (const fs::filesystem_error&) {
        return false;
    }
}

std::vector<std::string> Game::listSaves() {
    std::vector<std::string> saves;
    try {
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.path().extension() == ".sav") {
                saves.push_back(entry.path().filename().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cout << "Ошибка при поиске сохранений: " << e.what() << std::endl;
    }
    return saves;
}

bool Game::validateSaveFile(const std::string& filename) {
    try {
        SaveFile file(filename, std::ios::in | std::ios::binary);
        auto& stream = file.get();
        
        std::string header;
        std::getline(stream, header);
        file.close();
        
        return header == "GAME_SAVE_v1.0";
    } catch (...) {
        return false;
    }
}
