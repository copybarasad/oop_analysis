#include "MenuSystem.h"
#include "PlayerUpgrade.h"
#include <iostream>
#include <limits>

/**
 * @file MenuSystem.cpp
 * @brief Реализация системы меню (ИСПРАВЛЕНО - убраны рамки)
 */

/**
 * @brief Главное меню (БЕЗ РАМОК)
 */
MenuSystem::MainMenuChoice MenuSystem::showMainMenu() {
    std::cout << "\n=== DUNGEON CRAWLER ===\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    std::cout << "Choice: ";
    
    int choice = getChoice(1, 3);
    
    switch (choice) {
        case 1: return MainMenuChoice::NEW_GAME;
        case 2: return MainMenuChoice::LOAD_GAME;
        case 3: return MainMenuChoice::EXIT;
        default: return MainMenuChoice::EXIT;
    }
}

/**
 * @brief Меню загрузки (БЕЗ РАМОК)
 */
std::string MenuSystem::showLoadMenu() {
    std::cout << "\nEnter save name (or press Enter to cancel): ";
    std::string saveName = getString();
    return saveName;
}

/**
 * @brief Меню улучшений (БЕЗ РАМОК)
 */
const Upgrade* MenuSystem::showUpgradeMenu(const PlayerUpgradeSystem* upgradeSystem) {
    if (!upgradeSystem) return nullptr;
    
    std::cout << "\n=== LEVEL UP - CHOOSE UPGRADE ===\n";
    
    const auto& upgrades = upgradeSystem->getAllUpgrades();
    
    for (size_t i = 0; i < upgrades.size(); ++i) {
        std::cout << (i + 1) << ". " << upgrades[i].name << " - " << upgrades[i].description << "\n";
    }
    
    std::cout << "Choice: ";
    int choice = getChoice(1, static_cast<int>(upgrades.size()));
    
    return upgradeSystem->getUpgrade(choice);
}

/**
 * @brief Меню поражения (БЕЗ РАМОК)
 */
MenuSystem::DefeatMenuChoice MenuSystem::showDefeatMenu() {
    std::cout << "\n*** GAME OVER ***\n";
    std::cout << "1. Start New Game\n";
    std::cout << "2. Exit to Main Menu\n";
    std::cout << "Choice: ";
    
    int choice = getChoice(1, 2);
    
    return (choice == 1) ? DefeatMenuChoice::NEW_GAME : DefeatMenuChoice::EXIT;
}

/**
 * @brief Запросить переход на следующий уровень
 */
bool MenuSystem::askNextLevel() {
    std::cout << "\nProceed to next level? (Y/N): ";
    std::string input = getString();
    return (input == "Y" || input == "y");
}

/**
 * @brief Запросить сохранение при выходе (НОВОЕ)
 */
bool MenuSystem::askSaveBeforeExit() {
    std::cout << "\nSave game before exit? (Y/N): ";
    std::string input = getString();
    return (input == "Y" || input == "y");
}

/**
 * @brief Запросить имя сохранения (НОВОЕ)
 */
std::string MenuSystem::askSaveName() {
    std::cout << "Enter save name: ";
    std::string saveName = getString();
    if (saveName.empty()) {
        return "quicksave";
    }
    return saveName;
}

/**
 * @brief Получить выбор пользователя
 */
int MenuSystem::getChoice(int min, int max) {
    int choice;
    
    while (true) {
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again: ";
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice >= min && choice <= max) {
            return choice;
        }
        
        std::cout << "Choice must be between " << min << " and " << max << ". Try again: ";
    }
}

/**
 * @brief Получить строку от пользователя
 */
std::string MenuSystem::getString() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}


