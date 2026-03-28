#include "GameRenderer.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "GameField.h"
#include <iostream>
#include <iomanip>

/**
 * @file GameRenderer.cpp
 * @brief Реализация отрисовки игрового состояния
 */

const std::string GameRenderer::SEPARATOR = "========================================";
const std::string GameRenderer::HEADER_CHAR = "=";

/**
 * @brief Вывести приветственное сообщение
 */
void GameRenderer::printWelcome() const {
    std::cout << "\n";
    std::cout << SEPARATOR << "\n";
    std::cout << "    WELCOME TO DUNGEON CRAWLER!\n";
    std::cout << SEPARATOR << "\n";
    std::cout << "Goal: Defeat all enemies and towers!\n";
    std::cout << "Type 'H' for help or 'Q' to quit.\n";
    std::cout << SEPARATOR << "\n\n";
}

/**
 * @brief Вывести игровое поле
 * @param game Указатель на игру
 */
void GameRenderer::printField(const Game* game) const {
    if (!game || !game->getField()) {
        printError("Cannot render field - game or field is null!");
        return;
    }
    
    const GameField* field = game->getField();
    const Player* player = game->getPlayer();
    
    if (!player) {
        printError("Player is null!");
        return;
    }
    
    auto [px, py] = player->getPosition();
    
    // Заголовок с координатами X
    std::cout << "\n   ";
    for (int x = 0; x < field->getWidth(); ++x) {
        std::cout << std::setw(2) << (x % 10);
    }
    std::cout << "\n   ";
    for (int x = 0; x < field->getWidth(); ++x) {
        std::cout << "--";
    }
    std::cout << "\n";
    
    // Основное поле
    for (int y = 0; y < field->getHeight(); ++y) {
        std::cout << std::setw(2) << y << "|";
        
        for (int x = 0; x < field->getWidth(); ++x) {
            bool found = false;
            
            // Игрок
            if (x == px && y == py) {
                std::cout << "P ";
                continue;
            }
            
            // Враги
            for (const auto& enemy : game->getEnemies()) {
                if (!enemy->isAlive()) continue;
                auto [ex, ey] = enemy->getPosition();
                if (x == ex && y == ey) {
                    std::cout << "E ";
                    found = true;
                    break;
                }
            }
            if (found) continue;
            
            // Союзники
            for (const auto& ally : game->getAllies()) {
                if (!ally->isAlive()) continue;
                auto [ax, ay] = ally->getPosition();
                if (x == ax && y == ay) {
                    std::cout << "A ";
                    found = true;
                    break;
                }
            }
            if (found) continue;
            
            // Башни
            for (const auto& tower : game->getTowers()) {
                if (!tower->isAlive()) continue;
                auto [tx, ty] = tower->getPosition();
                if (x == tx && y == ty) {
                    std::cout << "T ";
                    found = true;
                    break;
                }
            }
            if (found) continue;
            
            // Ловушки
            for (const auto& trap : game->getTraps()) {
                if (!trap->isAlive()) continue;
                auto [trx, try_] = trap->getPosition();
                if (x == trx && y == try_) {
                    std::cout << "X ";
                    found = true;
                    break;
                }
            }
            if (found) continue;
            
            // Пустая клетка или стена
            if (field->isCellPassable(x, y)) {
                std::cout << ". ";
            } else {
                std::cout << "# ";
            }
        }
        std::cout << "\n";
    }
    
    std::cout << "\nP=Player, E=Enemy, A=Ally, T=Tower, X=Trap, #=Wall, .=Empty\n";
}

/**
 * @brief Вывести статус игрока
 * @param player Указатель на игрока
 */
void GameRenderer::printPlayerStatus(const Player* player) const {
    if (!player) {
        printError("Player is null!");
        return;
    }
    
    std::cout << "\n" << SEPARATOR << "\n";
    std::cout << "         PLAYER STATUS\n";
    std::cout << SEPARATOR << "\n";
    
    auto [px, py] = player->getPosition();
    
    std::cout << "Position: (" << px << ", " << py << ")\n";
    std::cout << "Health: " << player->getHealth() << "/" << player->getMaxHealth() << "\n";
    std::cout << "Damage: " << player->getDamage() << "\n";
    std::cout << "Attack Mode: " << (player->getAttackMode() == AttackMode::MELEE ? 
                                      "MELEE (40 dmg)" : "RANGED (20 dmg, range 3)") << "\n";
    std::cout << "Score: " << player->getScore() << "\n";
    std::cout << "Enemies Killed: " << player->getEnemiesKilled() << "\n";
    
    std::cout << SEPARATOR << "\n";
}

/**
 * @brief Вывести содержимое руки заклинаний
 * @param spellHand Указатель на руку заклинаний
 */
void GameRenderer::printSpellHand(const SpellHand* spellHand) const {
    if (!spellHand) {
        printError("SpellHand is null!");
        return;
    }
    
    std::cout << "\n" << SEPARATOR << "\n";
    std::cout << "    SPELL HAND (" << spellHand->getSpellCount() << "/" 
              << spellHand->getMaxHandSize() << ")\n";
    std::cout << SEPARATOR << "\n";
    
    if (spellHand->getBoostStacks() > 0) {
        std::cout << "Active Boost Stacks: " << spellHand->getBoostStacks() << "\n";
        std::cout << "---\n";
    }
    
    if (spellHand->isEmpty()) {
        std::cout << "Hand is empty!\n";
    } else {
        const auto& spells = spellHand->getSpells();
        for (size_t i = 0; i < spells.size(); ++i) {
            std::cout << i << ": " << spells[i]->getName() 
                      << " - " << spells[i]->getDescription() << "\n";
        }
    }
    
    std::cout << SEPARATOR << "\n";
}

/**
 * @brief Вывести общую информацию об игре
 * @param game Указатель на игру
 */
void GameRenderer::printGameInfo(const Game* game) const {
    if (!game) {
        printError("Game pointer is null!");
        return;
    }
    
    std::cout << "\n" << SEPARATOR << "\n";
    std::cout << "         GAME INFORMATION\n";
    std::cout << SEPARATOR << "\n";
    
    // Уровень
    std::cout << "Level: " << game->getCurrentLevel() << "\n";
    
    // Враги
    std::cout << "Enemies Alive: " << game->getEnemies().size() << "\n";
    
    // Союзники
    if (game->getAllies().size() > 0) {
        std::cout << "Active Allies: " << game->getAllies().size() << "\n";
    }
    
    // Башни
    if (game->getTowers().size() > 0) {
        std::cout << "Enemy Towers: " << game->getTowers().size() << "\n";
    }
    
    // Ловушки
    if (game->getTraps().size() > 0) {
        std::cout << "Active Traps: " << game->getTraps().size() << "\n";
    }
    
    // Размер поля
    const GameField* field = game->getField();
    if (field) {
        std::cout << "Field Size: " << field->getWidth() << " x " << field->getHeight() << "\n";
    }
    
    std::cout << SEPARATOR << "\n";
}

/**
 * @brief Вывести сообщение об успехе
 * @param message Сообщение
 */
void GameRenderer::printSuccess(const std::string& message) const {
    std::cout << "[SUCCESS] " << message << "\n";
}

/**
 * @brief Вывести сообщение об ошибке
 * @param message Сообщение
 */
void GameRenderer::printError(const std::string& message) const {
    std::cout << "[ERROR] " << message << "\n";
}

/**
 * @brief Вывести информационное сообщение
 * @param message Сообщение
 */
void GameRenderer::printInfo(const std::string& message) const {
    std::cout << "[INFO] " << message << "\n";
}

/**
 * @brief Вывести победное сообщение
 * @param player Указатель на игрока
 */
void GameRenderer::printVictory(const Player* player) const {
    std::cout << "\n" << SEPARATOR << "\n";
    std::cout << "         VICTORY!\n";
    std::cout << SEPARATOR << "\n";
    std::cout << "All enemies defeated!\n";
    
    if (player) {
        std::cout << "Final Score: " << player->getScore() << "\n";
        std::cout << "Enemies Killed: " << player->getEnemiesKilled() << "\n";
    }
    
    std::cout << SEPARATOR << "\n";
}

/**
 * @brief Вывести сообщение о поражении
 */
void GameRenderer::printGameOver() const {
    std::cout << "\n" << SEPARATOR << "\n";
    std::cout << "         GAME OVER!\n";
    std::cout << SEPARATOR << "\n";
    std::cout << "You have been defeated...\n";
    std::cout << SEPARATOR << "\n";
}


