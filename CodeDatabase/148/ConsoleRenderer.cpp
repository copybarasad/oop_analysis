#include "ConsoleRenderer.h"
#include "Game.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "SpellHand.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

/**
 * @file ConsoleRenderer.cpp
 * @brief Реализация консольной отрисовки
 */

/**
 * @brief Отрисовать полное состояние игры
 */
void ConsoleRenderer::renderGame(const Game* game) {
    if (!game) return;

    renderField(game);
    renderCompactLegend();
    renderEnemyDistances(game);
    renderPlayerStatus(game->getPlayer());
}

/**
 * @brief Отрисовать игровое поле (без легенды внизу)
 */
void ConsoleRenderer::renderField(const Game* game) {
    if (!game || !game->getField()) return;

    const GameField* field = game->getField();
    const Player* player = game->getPlayer();
    if (!player) return;

    auto [px, py] = player->getPosition();

    // Заголовок
    std::cout << "\n   ";
    for (int x = 0; x < field->getWidth(); ++x) {
        std::cout << std::setw(2) << (x % 10);
    }
    std::cout << "\n   ";
    for (int x = 0; x < field->getWidth(); ++x) {
        std::cout << "--";
    }
    std::cout << "\n";

    // Поле
    for (int y = 0; y < field->getHeight(); ++y) {
        std::cout << std::setw(2) << y << "|";

        for (int x = 0; x < field->getWidth(); ++x) {
            bool found = false;

            if (x == px && y == py) {
                std::cout << "P ";
                continue;
            }

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

            if (field->isCellPassable(x, y)) {
                std::cout << ". ";
            } else {
                std::cout << "# ";
            }
        }
        std::cout << "\n";
    }
}

/**
 * @brief Краткая легенда
 */
void ConsoleRenderer::renderCompactLegend() const {
    std::cout << "\nP=Player | E=Enemy | A=Ally | T=Tower | X=Trap | #=Wall\n";
}

/**
 * @brief Расстояния до врагов и башен
 */
void ConsoleRenderer::renderEnemyDistances(const Game* game) {
    if (!game || !game->getPlayer()) return;

    const Player* player = game->getPlayer();
    auto [px, py] = player->getPosition();

    struct Target {
        std::string type;
        int x, y;
        int distance;
    };

    std::vector<Target> targets;

    // Враги
    for (const auto& enemy : game->getEnemies()) {
        if (!enemy->isAlive()) continue;
        auto [ex, ey] = enemy->getPosition();
        int dx = std::abs(px - ex);
        int dy = std::abs(py - ey);
        int dist = std::max(dx, dy); // Чебышевское расстояние
        targets.push_back({"E", ex, ey, dist});
    }

    // Башни
    for (const auto& tower : game->getTowers()) {
        if (!tower->isAlive()) continue;
        auto [tx, ty] = tower->getPosition();
        int dx = std::abs(px - tx);
        int dy = std::abs(py - ty);
        int dist = std::max(dx, dy);
        targets.push_back({"T", tx, ty, dist});
    }

    if (targets.empty()) return;

    std::sort(targets.begin(), targets.end(),
              [](const Target& a, const Target& b) { return a.distance < b.distance; });

    std::cout << "\nTargets: ";
    for (size_t i = 0; i < targets.size() && i < 5; ++i) {
        if (i > 0) std::cout << " | ";
        std::cout << targets[i].type << "(" << targets[i].x << "," << targets[i].y
                  << ") dist:" << targets[i].distance;
    }
    std::cout << "\n";
}

/**
 * @brief Отрисовать статус игрока
 */
void ConsoleRenderer::renderPlayerStatus(const Player* player) {
    if (!player) return;

    auto [px, py] = player->getPosition();
    std::cout << "\nHP: " << player->getHealth() << "/" << player->getMaxHealth()
              << " | Pos: (" << px << "," << py << ") | Mode: "
              << (player->getAttackMode() == AttackMode::MELEE ? "MELEE(40)" : "RANGED(20,r3)")
              << " | Score: " << player->getScore()
              << " | Kills: " << player->getEnemiesKilled() << "\n";
}

/**
 * @brief Отрисовать руку заклинаний
 */
void ConsoleRenderer::renderSpellHand(const SpellHand* spellHand) {
    if (!spellHand) return;

    std::cout << "\n--- SPELL HAND (" << spellHand->getSpellCount() << "/"
              << spellHand->getMaxHandSize() << ") ---\n";

    if (spellHand->getBoostStacks() > 0) {
        std::cout << "Boost Stacks: " << spellHand->getBoostStacks() << "\n";
    }

    if (spellHand->isEmpty()) {
        std::cout << "Hand is empty!\n";
    } else {
        const auto& spells = spellHand->getSpells();
        for (size_t i = 0; i < spells.size(); ++i) {
            std::cout << i << ". " << spells[i]->getName()
                      << " - " << spells[i]->getDescription() << "\n";
        }
    }
}

/**
 * @brief Показать сообщение
 */
void ConsoleRenderer::showMessage(const std::string& message) {
    std::cout << message << "\n";
}

/**
 * @brief Показать ошибку
 */
void ConsoleRenderer::showError(const std::string& error) {
    std::cout << "[ERROR] " << error << "\n";
}

/**
 * @brief Приветствие
 */
void ConsoleRenderer::printWelcome() const {
    std::cout << "\n=== DUNGEON CRAWLER ===\n";
    std::cout << "Goal: Defeat all enemies and towers!\n";
}

/**
 * @brief Победа
 */
void ConsoleRenderer::printVictory(const Player* player) const {
    std::cout << "\n*** VICTORY! ***\n";
    if (player) {
        std::cout << "Score: " << player->getScore()
                  << " | Kills: " << player->getEnemiesKilled() << "\n";
    }
}

/**
 * @brief Поражение
 */
void ConsoleRenderer::printGameOver() const {
    std::cout << "\n*** GAME OVER! ***\n";
}


