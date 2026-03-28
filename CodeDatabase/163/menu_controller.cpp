#include "menu_controller.hpp"
#include "process_game.hpp"
#include "save_game.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>

void MenuController::waitForAnyKey() const {
    std::cout << "\nНажмите Enter чтобы продолжить...\n";
    std::cin.ignore();
    std::cin.get();
}

void MenuController::showMessage(const std::string& message) const {
    std::cout << message << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

void MenuController::runMainMenu() {
    bool exitProgram = false;
    
    while (!exitProgram) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "=== ГЛАВНОЕ МЕНЮ ===\n\n";
        std::cout << "1. Новая игра\n";
        std::cout << "2. Загрузить игру\n";
        std::cout << "3. Сохранения\n";
        std::cout << "4. Управление\n";
        std::cout << "0. Выход\n\n";
        std::cout << "Выберите опцию: ";
        
        char choice;
        std::cin >> choice;
        std::cin.ignore(1000, '\n');
        
        switch (choice) {
            case '1':
                {
                    ProcessGame gameProcess;
                    gameProcess.initializeNewGame();
                    gameProcess.startGameLoop();
                    // После завершения игры возвращаемся в меню
                }
                break;
                
            case '2':
                showLoadMenu();
                break;
                
            case '3':
                showSaveMenu();
                break;
                
            case '4':
                showHelp();
                waitForAnyKey();
                break;
                
            case '0':
                exitProgram = true;
                break;
                
            default:
                std::cout << "Неверный выбор.\n";
                waitForAnyKey();
                break;
        }
    }
}

void MenuController::showHelp() const {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "=== УПРАВЛЕНИЕ ===\n\n";
    std::cout << "ВО ВРЕМЯ ИГРЫ:\n";
    std::cout << "W/A/S/D     - Движение\n";
    std::cout << "ПРОБЕЛ+W/A/S/D - Атака\n";
    std::cout << "Q           - Сменить оружие\n";
    std::cout << "1-9         - Использовать заклинание\n";
    std::cout << "B           - Купить случайное заклинание\n";
    std::cout << "E           - Пропустить ход\n";
    std::cout << "M           - Меню паузы\n";
    std::cout << "0           - Выйти в меню\n\n";
    
    std::cout << "СИСТЕМА ЗАКЛИНАНИЙ:\n";
    std::cout << "- Заклинания летят как снаряды\n";
    std::cout << "- Выберите номер заклинания (1-9)\n";
    std::cout << "- Выберите направление (W/A/S/D)\n";
    std::cout << "- Снаряд летит до цели/препятствия\n";
    std::cout << "- Прямые заклинания поражают одну цель\n";
    std::cout << "- Областные заклинания взрываются по площади\n";
    std::cout << "- Убивайте врагов для получения очков заклинаний\n";
}

void MenuController::showSaveMenu() const {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "=== УПРАВЛЕНИЕ СОХРАНЕНИЯМИ ===\n\n";
    
    try {
        auto saves = SaveGame::listSaves();
        if (saves.empty()) {
            std::cout << "Нет сохраненных игр.\n";
        } else {
            std::cout << "Сохраненные игры:\n";
            for (size_t i = 0; i < saves.size(); ++i) {
                try {
                    auto size = std::filesystem::file_size(saves[i]);
                    std::cout << i + 1 << ". " << saves[i] << " (" << size << " байт)\n";
                } catch (const std::exception&) {
                    std::cout << i + 1 << ". " << saves[i] << " (ошибка чтения)\n";
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка при чтении сохранений: " << e.what() << "\n";
    }
    
    this->waitForAnyKey();
}

void MenuController::showLoadMenu() {
    bool backToMainMenu = false;
    
    while (!backToMainMenu) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "=== ЗАГРУЗКА ИГРЫ ===\n\n";
        std::cout << "1. Загрузить последнюю игру (savegame.dat)\n";
        std::cout << "2. Выбрать файл сохранения\n";
        std::cout << "0. Назад в главное меню\n\n";
        std::cout << "Выберите опцию: ";
        
        char choice;
        std::cin >> choice;
        std::cin.ignore(1000, '\n');
        
        switch (choice) {
            case '1':
                {
                    ProcessGame gameProcess;
                    if (gameProcess.loadGame()) {
                        std::cout << "Загрузка успешна! Запуск игры...\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        gameProcess.startGameLoop();
                    } else {
                        std::cout << "Не удалось загрузить сохранение.\n";
                        waitForAnyKey();
                    }
                    backToMainMenu = true;
                }
                break;
                
            case '2':
                {
                    std::cout << "Введите имя файла сохранения: ";
                    std::string filename;
                    std::cin >> filename;
                    std::cin.ignore(1000, '\n');
                    
                    ProcessGame gameProcess;
                    if (gameProcess.loadGame(filename)) {
                        std::cout << "Загрузка успешна! Запуск игры...\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        gameProcess.startGameLoop();
                    } else {
                        std::cout << "Не удалось загрузить сохранение из файла " 
                                  << filename << "\n";
                        waitForAnyKey();
                    }
                    backToMainMenu = true;
                }
                break;
                
            case '0':
                backToMainMenu = true;
                break;
                
            default:
                std::cout << "Неверный выбор.\n";
                waitForAnyKey();
                break;
        }
    }
}

void MenuController::showLevelCompleteMenu() {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "=== УРОВЕНЬ ПРОЙДЕН! ===\n\n";
    std::cout << "Поздравляем! Вы прошли уровень " 
              << this->gameController.getCurrentLevel() << "!\n";
    std::cout << "Ходов потрачено: " << this->gameController.getCurrentTurn() << "\n";
    std::cout << "Очков набрано: " << this->gameController.getScore() << "\n\n";
    
    std::cout << "1. Следующий уровень\n";
    std::cout << "2. Сохранить игру\n";
    std::cout << "3. Главное меню\n";
    std::cout << "0. Выйти\n\n";
    std::cout << "Выберите опцию: ";
    
    char choice;
    std::cin >> choice;
    std::cin.ignore(1000, '\n');
    
    switch (choice) {
        case '1':
            this->gameController.initializeNewGame();
            break;
            
        case '2':
            this->gameController.saveGame();
            break;
            
        case '3':
            break;
            
        default:
            std::cout << "Неверный выбор.\n";
            this->waitForAnyKey();
            break;
    }
}

void MenuController::showGameOverMenu() {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "=== ИГРА ОКОНЧЕНА ===\n\n";
    std::cout << "Вы были побеждены!\n";
    std::cout << "Прогресс: Уровень " << this->gameController.getCurrentLevel() 
              << ", Счет: " << this->gameController.getScore() << "\n\n";
    
    std::cout << "1. Попробовать снова\n";
    std::cout << "2. Главное меню\n";
    std::cout << "3. Сохранить результат\n";
    std::cout << "0. Выйти\n\n";
    std::cout << "Выберите опцию: ";
    
    char choice;
    std::cin >> choice;
    std::cin.ignore(1000, '\n');
    
    switch (choice) {
        case '1':
            this->gameController.initializeNewGame();
            break;
            
        case '2':
            break;
            
        case '3':
            this->gameController.saveGame();
            break;
            
        default:
            std::cout << "Неверный выбор.\n";
            this->waitForAnyKey();
            break;
    }
}