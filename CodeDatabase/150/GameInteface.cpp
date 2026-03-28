#include "GameInterface.hpp"
#include "GameManager.hpp"
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <windows.h>
#include <stdexcept>

GameInterface::GameInterface() : gameManager(std::make_unique<GameManager>(this)) {}

GameInterface::~GameInterface() = default;

void GameInterface::run() {
    try {
        gameManager->startGame();
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        std::cout << "Нажмите любую клавишу для выхода...";
        std::cin.get();
    }
    catch (...) {
        std::cerr << "Неизвестная критическая ошибка!" << std::endl;
        std::cout << "Нажмите любую клавишу для выхода...";
        std::cin.get();
    }
    
    std::cout << "Спасибо за игру!\n";
}

void GameInterface::clearScreen() {
    system("cls");
}

void GameInterface::showMainMenu() {
    displayMainMenu();
}

void GameInterface::showGameState() {
    if (gameManager->getCurrentState()) {
        displayGameState(*gameManager->getCurrentState());
    }
}

void GameInterface::showGameOver() {
    if (gameManager->getCurrentState()) {
        displayGameOver(*gameManager->getCurrentState());
    }
}

void GameInterface::showVictory() {
    if (gameManager->getCurrentState()) {
        displayVictory(*gameManager->getCurrentState());
    }
}

void GameInterface::showLevelTransition() {
    if (gameManager->getCurrentState()) {
        displayLevelTransition(*gameManager->getCurrentState());
    }
}

void GameInterface::showUpgradePhase() {
    if (gameManager->getCurrentState()) {
        displayUpgradePhase(*gameManager->getCurrentState(), const_cast<UpgradeSystem&>(gameManager->getUpgradeSystem()));
    }
}

void GameInterface::showSpellTargetSelection(ISpell* spell, int targetX, int targetY) {
    if (gameManager->getCurrentState()) {
        displaySpellTargetSelection(*gameManager->getCurrentState(), spell, targetX, targetY);
    }
}

void GameInterface::showRangedAttackTarget(int targetX, int targetY) {
    if (gameManager->getCurrentState()) {
        displayRangedAttackTarget(*gameManager->getCurrentState(), targetX, targetY);
    }
}

void GameInterface::showMessage(const std::string& message) {
    displayMessage(message);
}

void GameInterface::displayMainMenu() {
    clearScreen();
    std::cout << "=== RPG GAME ===\n";
    std::cout << "1. Новая игра\n";
    std::cout << "2. Загрузить игру\n";
    std::cout << "3. Выйти\n";
    std::cout << "Выберите опцию: ";
}

void GameInterface::displayGameState(const GameState& state) {
    clearScreen();
    std::cout << "=== RPG GAME ===\n";
    displayField(state.field);
    
    std::cout << "\n=== ГЕРОЙ ===\n";
    std::cout << "Уровень: " << state.currentLevel << "\n";
    std::cout << "Здоровье: " << state.player.getHealth() << "/" 
              << state.player.getMaxHealth() << "\n";
    std::cout << "Мана: " << state.player.getMana() << "/" 
              << state.player.getMaxMana() << "\n";
    std::cout << "Очки: " << state.player.getScore() << "\n";
    std::cout << "Тип атаки: " << (state.player.getAttackType() == AttackType::Melee ? "Ближний" : "Дальний") << "\n";
    
    std::cout << "\n=== ЗАКЛИНАНИЯ ===\n";
    const SpellHand& hand = state.player.getSpellHand();
    for (int i = 0; i < hand.getSpellCount(); ++i) {
        ISpell* spell = hand.getSpell(i);
        if (spell) {
            std::cout << i + 1 << ". ";
            switch (spell->getType()) {
                case SpellType::DirectDamage: std::cout << "Прямой урон"; break;
                case SpellType::AreaDamage: std::cout << "Урон по площади"; break;
                case SpellType::Trap: std::cout << "Ловушка"; break;
                case SpellType::Summon: std::cout << "Призыв"; break;
                case SpellType::Enhancement: std::cout << "Улучшение"; break;
            }
            std::cout << " (" << spell->getManaCost() << " маны)\n";
        }
    }
    
    std::cout << "\n=== ВАШ ХОД ===\n";
    std::cout << "WASD - движение, F - дальняя атака, 1-5 - заклинания\n";
    std::cout << "T - сменить тип атаки, B - купить заклинание, O - сохранить, L - загрузить, Q - выйти\n";
}

void GameInterface::displayField(const Field& field) {
    std::cout << "┌";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "──";
    }
    std::cout << "┐\n";
    
    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << "│";
        for (int x = 0; x < field.getWidth(); ++x) {
            const Cell& cell = field.getCell(x, y);
            
            if (cell.hasPlayer()) {
                std::cout << "P ";
            }
            else if (cell.hasEnemy()) {
                std::cout << "E ";
            }
            else if (cell.hasAlly()) {
                std::cout << "A ";
            }
            else if (cell.hasBuilding()) {
                std::cout << "B ";
            }
            else {
                switch (cell.getType()) {
                    case CellType::Empty: std::cout << ". "; break;
                    case CellType::Impassable: std::cout << "# "; break;
                    case CellType::Slowing: std::cout << "~ "; break;
                }
            }
        }
        std::cout << "│\n";
    }
    
    std::cout << "└";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "──";
    }
    std::cout << "┘\n";
}

void GameInterface::displayFieldWithTarget(const Field& field, int targetX, int targetY) {
    std::cout << "┌";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "──";
    }
    std::cout << "┐\n";
    
    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << "│";
        for (int x = 0; x < field.getWidth(); ++x) {
            const Cell& cell = field.getCell(x, y);
            
            if (x == targetX && y == targetY) {
                std::cout << "X ";
            }
            else if (cell.hasPlayer()) {
                std::cout << "P ";
            }
            else if (cell.hasEnemy()) {
                std::cout << "E ";
            }
            else if (cell.hasAlly()) {
                std::cout << "A ";
            }
            else if (cell.hasBuilding()) {
                std::cout << "B ";
            }
            else {
                switch (cell.getType()) {
                    case CellType::Empty:
                        std::cout << ". ";
                        break;
                    case CellType::Impassable:
                        std::cout << "# ";
                        break;
                    case CellType::Slowing:
                        std::cout << "~ ";
                        break;
                }
            }
        }
        std::cout << "│\n";
    }
    
    std::cout << "└";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "──";
    }
    std::cout << "┘\n";
}

void GameInterface::displayGameOver(const GameState& state) {
    clearScreen();
    std::cout << "=== ИГРА ОКОНЧЕНА ===\n";
    std::cout << "Ваш герой погиб...\n";
    std::cout << "Достигнутый уровень: " << state.currentLevel << "\n";
    std::cout << "Итоговый счет: " << state.player.getScore() << "\n\n";
    
    std::cout << "1. Начать заново\n";
    std::cout << "2. Выйти в главное меню\n";
    std::cout << "3. Выйти из игры\n";
    std::cout << "Выберите опцию: ";
}

void GameInterface::displayVictory(const GameState& state) {
    clearScreen();
    std::cout << "=== ПОБЕДА! ===\n";
    std::cout << "Вы прошли все уровни!\n";
    std::cout << "Итоговый счет: " << state.player.getScore() << "\n\n";
    
    std::cout << "1. Начать заново\n";
    std::cout << "2. Выйти из игры\n";
    std::cout << "Выберите опцию: ";
}

void GameInterface::displayLevelTransition(const GameState& state) {
    clearScreen();
    std::cout << "=== УРОВЕНЬ " << state.currentLevel << " ПРОЙДЕН! ===\n";

    SpellHand& hand = const_cast<SpellHand&>(state.player.getSpellHand());
    int spellsToRemove = hand.getSpellCount() / 2;
    
    std::cout << "Удалено заклинаний: " << spellsToRemove << "\n";
    for (int i = 0; i < spellsToRemove; ++i) {
        if (hand.getSpellCount() > 0) {
            hand.removeSpell(rand() % hand.getSpellCount());
        }
    }
    
    std::cout << "Переход на следующий уровень...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void GameInterface::displayUpgradePhase(const GameState& state, UpgradeSystem& upgradeSystem) {
    clearScreen();
    std::cout << "=== ПРОКАЧКА ГЕРОЯ ===\n";
    std::cout << "Уровень " << state.currentLevel + 1 << "\n\n";
    
    upgradeSystem.showUpgradeMenu(const_cast<Hero&>(state.player), const_cast<SpellHand&>(state.player.getSpellHand()));
}

void GameInterface::displaySpellTargetSelection(const GameState& state, ISpell* spell, int targetX, int targetY) {
    clearScreen();
    std::cout << "=== ВЫБОР ЦЕЛИ ===\n";
    displayFieldWithTarget(state.field, targetX, targetY);
    std::cout << "Цель: (" << targetX << ", " << targetY << ")\n";
    std::cout << "Заклинание: ";
    switch (spell->getType()) {
        case SpellType::DirectDamage: std::cout << "Прямой урон"; break;
        case SpellType::AreaDamage: std::cout << "Урон по площади"; break;
        case SpellType::Trap: std::cout << "Ловушка"; break;
        case SpellType::Summon: std::cout << "Призыв"; break;
        case SpellType::Enhancement: std::cout << "Улучшение"; break;
    }
    std::cout << " (" << spell->getManaCost() << " маны)\n";
    std::cout << "Управление: WASD - перемещение курсора, Enter - применить, ESC - отмена\n";
}

void GameInterface::displayRangedAttackTarget(const GameState& state, int targetX, int targetY) {
    clearScreen();
    std::cout << "=== ДАЛЬНОБОЙНАЯ АТАКА ===" << std::endl;
    displayFieldWithTarget(state.field, targetX, targetY);
    std::cout << "Цель: (" << targetX << ", " << targetY << ")" << std::endl;
    std::cout << "Дистанция: " << (std::abs(state.player.getPositionX() - targetX) + 
                                std::abs(state.player.getPositionY() - targetY)) << std::endl;
    std::cout << "Максимальная дистанция: " << state.player.getRangedRange() << std::endl;
    std::cout << "Урон: " << state.player.getDamage() << std::endl;
    std::cout << "Управление: WASD - перемещение курсора, Enter - атаковать, ESC - отмена\n";
}

void GameInterface::displayMessage(const std::string& message) {
    std::cout << message << std::endl;
}