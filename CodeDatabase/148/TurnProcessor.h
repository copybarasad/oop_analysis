#ifndef TURNPROCESSOR_H
#define TURNPROCESSOR_H

#include "Game.h"
#include <cmath>

/**
 * @file TurnProcessor.h
 * @brief Обработчик ходов игры
 * 
 * Инкапсулирует логику обработки ходов игрока, врагов, союзников и башен.
 */
class TurnProcessor {
private:
    Game* game;  ///< Указатель на игру
    
public:
    /**
     * @brief Конструктор
     * @param game Указатель на игру
     */
    explicit TurnProcessor(Game* game) : game(game) {}
    
    /**
     * @brief Обработка хода игрока
     * @param direction Направление движения
     */
    void processPlayerTurn(Direction direction) {
        if (!game->player->isAlive()) {
            game->gameOver();
            return;
        }

        auto [currentX, currentY] = game->player->getPosition();
        int newX = currentX;
        int newY = currentY;
        
        // Вычисляем новую позицию
        if (direction == Direction::UP) newY--;
        else if (direction == Direction::DOWN) newY++;
        else if (direction == Direction::LEFT) newX--;
        else if (direction == Direction::RIGHT) newX++;
        
        // Проверка валидности хода
        if (!game->isValidMove(newX, newY)) {
            return;
        }

        // Проверяем есть ли враг на целевой клетке
        Enemy* targetEnemy = findEnemyAt(newX, newY);
        
        if (targetEnemy) {
            handleCombat(targetEnemy);
            game->processAfterPlayerAction();
            return;
        }

        // Перемещаем игрока
        game->player->move(newX, newY);
        
        // Дальняя атака если включена
        handleRangedAttack(currentX, currentY);
        
        game->processAfterPlayerAction();
    }

    /**
     * @brief Обработка ходов врагов
     */
    void processEnemyTurns() {
        bool playerAttackedThisTurn = false;
        auto [px, py] = game->player->getPosition();
        
        for (auto& enemy : game->enemies) {
            if (!enemy->isAlive()) continue;
            
            auto [ex, ey] = enemy->getPosition();
            int dx = px - ex;
            int dy = py - ey;
            
            // Враг рядом с игроком - атакуем
            bool isAdjacentCardinal = (dx == 0 && (dy == 1 || dy == -1)) || 
                                      (dy == 0 && (dx == 1 || dx == -1));
            
            if (isAdjacentCardinal) {
                if (!playerAttackedThisTurn) {
                    enemy->attack(game->player.get());
                    playerAttackedThisTurn = true;
                }
                continue;
            }

            // Двигаемся к игроку
            moveEnemyTowardsPlayer(enemy.get(), px, py);
        }
    }

    /**
     * @brief Обработка ходов союзников
     */
    void processAllyTurns() {
        for (auto& ally : game->allies) {
            if (ally->isAlive()) {
                ally->takeTurn(game);
            }
        }
    }

    /**
     * @brief Обработка ходов башен
     */
    void processTowerTurns() {
        for (auto& tower : game->towers) {
            if (tower->isAlive()) {
                tower->takeTurn(game);
            }
        }
    }

private:
    /**
     * @brief Находит врага на указанной позиции
     */
    Enemy* findEnemyAt(int x, int y) {
        for (auto& enemy : game->enemies) {
            if (!enemy->isAlive()) continue;
            auto [ex, ey] = enemy->getPosition();
            if (ex == x && ey == y) {
                return enemy.get();
            }
        }
        return nullptr;
    }

    /**
     * @brief Обработка ближнего боя
     */
    void handleCombat(Enemy* targetEnemy) {
        if (game->player->getAttackMode() == AttackMode::MELEE) {
            game->player->attack(targetEnemy);
        }
    }

    /**
     * @brief Обработка дальней атаки
     */
    void handleRangedAttack(int currentX, int currentY) {
        if (game->player->getAttackMode() != AttackMode::RANGED) return;
        
        for (auto& enemy : game->enemies) {
            if (!enemy->isAlive()) continue;
            
            auto [ex, ey] = enemy->getPosition();
            int dx = std::abs(currentX - ex);
            int dy = std::abs(currentY - ey);
            int distance = std::max(dx, dy);
            
            if (distance <= RANGED_ATTACK_RANGE && distance > 0) {
                game->player->attack(enemy.get());
                break;
            }
        }
    }

    /**
     * @brief Двигает врага к игроку
     */
    void moveEnemyTowardsPlayer(Enemy* enemy, int px, int py) {
        auto [ex, ey] = enemy->getPosition();
        int dx = px - ex;
        int dy = py - ey;
        
        int newX = ex;
        int newY = ey;
        
        // Определяем направление
        if (std::abs(dx) > std::abs(dy)) {
            newX += (dx > 0) ? 1 : -1;
        } else if (dy != 0) {
            newY += (dy > 0) ? 1 : -1;
        }

        // Проверяем валидность и что клетка свободна
        if (game->isValidMove(newX, newY) && !isOccupiedByEnemy(newX, newY, enemy)) {
            if (newX != px || newY != py) {
                enemy->move(newX, newY);
            }
        }
    }

    /**
     * @brief Проверяет занята ли клетка другим врагом
     */
    bool isOccupiedByEnemy(int x, int y, Enemy* excludeEnemy) {
        for (auto& other : game->enemies) {
            if (other.get() == excludeEnemy) continue;
            if (!other->isAlive()) continue;
            
            auto [ox, oy] = other->getPosition();
            if (ox == x && oy == y) {
                return true;
            }
        }
        return false;
    }
};

#endif // TURNPROCESSOR_H


