#include "ConsoleGame.h"
#include "Game.h"
#include "GameState.h"
#include "SaveLoadManager.h"
#include <iostream>
#include <limits>
#include <windows.h>

void ConsoleGame::run() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    std::cout << "=== Пошаговая стратегия ===" << std::endl;
    std::cout << "1. Новая игра" << std::endl;
    std::cout << "2. Загрузить игру" << std::endl;
    std::cout << "Выберите вариант: ";
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        game_.startNewGame();
        std::cout << "Новая игра успешно создана." << std::endl;
    } else if (choice == 2) {
        bool loaded = false;
        try {
            loaded = game_.loadGame("save.txt");
            if (loaded) {
                std::cout << "Игра успешно загружена из файла save.txt" << std::endl;
            }
        } 
        catch (const FileNotFoundException& e) {
            std::cout << "Файл сохранения 'save.txt' не найден. Запускается новая игра." << std::endl;
            game_.startNewGame();
            loaded = true;
        } 
        catch (const CorruptedSaveException& e) {
            std::cout << "Файл сохранения 'save.txt' поврежден или содержит некорректные данные. Запускается новая игра." << std::endl;
            game_.startNewGame();
            loaded = true;
        } 
        catch (const std::exception& e) {
            std::cout << "Ошибка при загрузке игры: " << e.what() << ". Запускается новая игра." << std::endl;
            game_.startNewGame();
            loaded = true;
        }
        
        if (!loaded) {
            std::cout << "Не удалось загрузить игру. Запускается новая игра." << std::endl;
            game_.startNewGame();
        }
    } else {
        std::cout << "Неверный выбор. Запускается новая игра." << std::endl;
        game_.startNewGame();
    }
    
    if (!game_.getState() || !game_.getState()->getField() || !game_.getState()->getPlayer()) {
        std::cout << "Критическая ошибка: игра не инициализирована корректно." << std::endl;
        return;
    }
    
    std::cout << "Игра готова к запуску. Уровень: " << game_.getCurrentLevel() << std::endl;
    
    while (game_.getStatus() == GameStatus::RUNNING) {
        displayGameState();
        processInput();
    }
    
    if (game_.getStatus() == GameStatus::PLAYER_WON) {
        std::cout << "Поздравляем! Вы выиграли!" << std::endl;
    } else if (game_.getStatus() == GameStatus::PLAYER_LOST) {
        std::cout << "Игра окончена. Вы проиграли." << std::endl;
    }
}

void ConsoleGame::displayGameState() const {
    auto state = game_.getState();
    if (!state) {
        std::cout << "Ошибка: состояние игры не доступно" << std::endl;
        return;
    }
    
    auto field = state->getField();
    auto player = state->getPlayer();
    
    if (!field || !player) {
        std::cout << "Ошибка: поле или игрок не инициализированы" << std::endl;
        return;
    }
    
    std::cout << "\n=== Уровень " << game_.getCurrentLevel() << " ===" << std::endl;
    displayPlayerInfo();
    displayField();
    displaySpells();
    displayMenu();
}

void ConsoleGame::displayPlayerInfo() const {
    auto state = game_.getState();
    if (!state) return;
    
    auto player = state->getPlayer();
    if (!player) return;
    
    std::cout << "Игрок: HP " << player->getHp() << "/" << player->getMaxHp();
    std::cout << " | Урон: " << player->getDamage();
    std::cout << " | Очки: " << player->getPoints();
    std::cout << " | Режим: " << (player->getAttackMode() == AttackMode::MELEE ? "Ближний" : "Дальний");
    std::cout << " | Дальность: " << player->getRangedRange();
    std::cout << " | Замедлен: " << (player->isSlowed() ? "Да" : "Нет");
    std::cout << std::endl;
}

void ConsoleGame::displayField() const {
    auto state = game_.getState();
    if (!state) return;
    
    auto field = state->getField();
    if (!field) return;
    
    std::cout << "\n";
    for (int y = 0; y < field->getHeight(); ++y) {
        for (int x = 0; x < field->getWidth(); ++x) {
            std::cout << getCellSymbol(x, y) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Легенда: P-игрок, E-враг, B-база, T-башня, A-союзник, #-стена, ~-замедление, X-ловушка" << std::endl;
}

char ConsoleGame::getCellSymbol(int x, int y) const {
    auto state = game_.getState();
    if (!state) return '.';
    
    auto field = state->getField();
    if (!field) return '.';
    
    const auto& cell = field->getCell(x, y);
    
    if (cell.getPlayer()) return 'P';
    if (cell.getEnemy()) return 'E';
    if (cell.getBase()) return 'B';
    if (cell.getTower()) return 'T';
    if (cell.getAlly()) return 'A';
    if (cell.hasTrap()) return 'X';
    
    switch (cell.getType()) {
        case CellType::WALL: return '#';
        case CellType::SLOW: return '~';
        default: return '.';
    }
}

void ConsoleGame::displaySpells() const {
    auto state = game_.getState();
    if (!state) return;
    
    auto player = state->getPlayer();
    if (!player) return;
    
    auto spells = player->getHand().getAvailableSpells();
    if (!spells.empty()) {
        std::cout << "\nЗаклинания:" << std::endl;
        for (size_t i = 0; i < spells.size(); ++i) {
            std::cout << i + 1 << ". " << spells[i]->getName() << std::endl;
        }
    } else {
        std::cout << "\nЗаклинаний нет" << std::endl;
    }
}

void ConsoleGame::displayMenu() const {
    std::cout << "\nКоманды:" << std::endl;
    std::cout << "W - движение вверх" << std::endl;
    std::cout << "A - движение влево" << std::endl;
    std::cout << "S - движение вниз" << std::endl;
    std::cout << "D - движение вправо" << std::endl;
    std::cout << "M - сменить режим атаки" << std::endl;
    std::cout << "C - использовать заклинание" << std::endl;
    std::cout << "E - закончить ход" << std::endl;
    std::cout << "Q - сохранить и выйти" << std::endl;
    std::cout << "Введите команду: ";
}

void ConsoleGame::processInput() {
    char command;
    std::cin >> command;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    bool moved = false;
    
    switch (std::tolower(command)) {
        case 'w': 
            moved = game_.movePlayer(0, -1); 
            break;
        case 's': 
            moved = game_.movePlayer(0, 1); 
            break;
        case 'a': 
            moved = game_.movePlayer(-1, 0); 
            break;
        case 'd': 
            moved = game_.movePlayer(1, 0); 
            break;
        case 'm': 
            game_.switchAttackMode();
            std::cout << "Режим атаки изменен!" << std::endl;
            break;
        case 'c':
            processSpellSelection();
            break;
        case 'e':
            game_.endTurn();
            std::cout << "Ход завершен!" << std::endl;
            break;
        case 'q':
            try {
                if (game_.saveGame("save.txt")) {
                    std::cout << "Игра успешно сохранена в файл save.txt. Выход..." << std::endl;
                } else {
                    std::cout << "Не удалось сохранить игру. Выход без сохранения..." << std::endl;
                }
            } 
            catch (const std::exception& e) {
                std::cout << "Ошибка сохранения: " << e.what() << std::endl;
                std::cout << "Выход без сохранения..." << std::endl;
            }
            exit(0);
            break;
        default:
            std::cout << "Неизвестная команда!" << std::endl;
            return;
    }
    
    if (moved) {
        std::cout << "Игрок переместился!" << std::endl;
    }
}

void ConsoleGame::processSpellSelection() {
    auto state = game_.getState();
    if (!state) return;
    
    auto player = state->getPlayer();
    if (!player) return;
    
    auto field = state->getField();
    if (!field) return;
    
    auto spells = player->getHand().getAvailableSpells();
    if (spells.empty()) {
        std::cout << "У вас нет заклинаний!" << std::endl;
        return;
    }
    
    std::cout << "Выберите заклинание (1-" << spells.size() << "): ";
    int spellIndex;
    std::cin >> spellIndex;
    
    if (std::cin.fail() || spellIndex < 1 || spellIndex > spells.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный номер заклинания!" << std::endl;
        return;
    }
    
    std::cout << "Введите координаты для заклинания (x y): ";
    int x, y;
    std::cin >> x >> y;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверные координаты!" << std::endl;
        return;
    }
    
    bool success = game_.useSpell(spellIndex - 1, x, y);
    if (success) {
        std::cout << "Заклинание '" << spells[spellIndex - 1]->getName() << "' применено!" << std::endl;
    } else {
        std::cout << "Не удалось применить заклинание!" << std::endl;
    }
}