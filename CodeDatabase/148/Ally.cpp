#include "Ally.h"
#include "Game.h"
#include "GameEventSystem.h"
#include "GameEvent.h"
#include "Enemy.h"
#include <cmath>
#include <climits>

/**
 * @file Ally.cpp
 * @brief Реализация класса союзника
 */

/**
 * @brief Конструктор союзника
 * @param posX X-координата начальной позиции
 * @param posY Y-координата начальной позиции
 * @param health Здоровье союзника
 * @param damage Урон при атаке
 * @param moveRange Максимальное расстояние движения за ход
 * 
 * Инициализирует союзника с вызовом конструктора базового класса GameEntity.
 */
Ally::Ally(int posX, int posY, int health, int damage, int moveRange) 
    : GameEntity(health, damage, posX, posY), moveRange(moveRange) {
}

/**
 * @brief Получить урон (переопределение с логированием)
 * @param amount Количество урона
 */
void Ally::takeDamage(int amount) {
    int oldHealth = health;
    
    // Вызываем базовый метод
    GameEntity::takeDamage(amount);
    
    // Логируем урон союзника
    auto [ax, ay] = getPosition();
    std::string allyDesc = "Ally at (" + std::to_string(ax) + "," + std::to_string(ay) + ")";
    DamageEvent event(EventType::ENEMY_DAMAGED, "Enemy", allyDesc, amount, health);
    GameEventSystem::getInstance().notifyEvent(event);
    
    // Если союзник умер
    if (health == 0 && oldHealth > 0) {
        EntityEvent deathEvent(EventType::ALLY_DIED, "Ally", ax, ay, 
                               "Ally at (" + std::to_string(ax) + "," + std::to_string(ay) + ") died!");
        GameEventSystem::getInstance().notifyEvent(deathEvent);
    }
}

/**
 * @brief Атаковать цель (переопределение с логированием)
 * @param target Указатель на цель
 */
void Ally::attack(GameEntity* target) {
    if (!target || !target->isAlive()) return;
    
    int targetHealthBefore = target->getHealth();
    target->takeDamage(this->damage);
    int targetHealthAfter = target->getHealth();
    
    int actualDamage = targetHealthBefore - targetHealthAfter;
    
    // Логируем атаку союзника
    auto [ax, ay] = getPosition();
    std::string allyDesc = "Ally at (" + std::to_string(ax) + "," + std::to_string(ay) + ")";
    std::string targetDesc = target->getType();
    
    // Используем общее событие (можно использовать PLAYER_ATTACK как "friendly attack")
    DamageEvent event(EventType::PLAYER_ATTACK, allyDesc, targetDesc, actualDamage, targetHealthAfter);
    GameEventSystem::getInstance().notifyEvent(event);
}

/**
 * @brief Выполнить ход союзника
 * @param game Указатель на игру
 * 
 * Союзник каждый ход:
 * 1. Ищет ближайшего врага
 * 2. Движется в его направлении
 * 3. Атакует если враг в пределах досягаемости
 */
void Ally::takeTurn(Game* game) {
    if (!game || !isAlive()) {
        return;
    }

    auto [nearestEnemy, distance] = findNearestEnemy(game);
    
    if (!nearestEnemy) {
        return;
    }
    
    // Если враг в радиусе атаки (соседняя клетка)
    if (distance <= 1) {
        attack(nearestEnemy);
        return;
    }

    // Двигаемся к врагу
    auto [enemyX, enemyY] = nearestEnemy->getPosition();
    
    int dx = enemyX - positionX;
    int dy = enemyY - positionY;
    
    // Нормализуем направление (выбираем основную ось)
    int moveX = 0, moveY = 0;
    
    if (std::abs(dx) > std::abs(dy)) {
        moveX = (dx > 0) ? 1 : -1;
    } else if (std::abs(dy) > 0) {
        moveY = (dy > 0) ? 1 : -1;
    }

    // Двигаемся на расстояние moveRange
    for (int i = 0; i < moveRange; ++i) {
        int newX = positionX + moveX;
        int newY = positionY + moveY;
        
        const GameField* field = game->getField();
        
        // Проверяем границы поля
        if (newX < 0 || newX >= field->getWidth() || 
            newY < 0 || newY >= field->getHeight()) {
            break;
        }

        bool occupied = false;
        
        // Проверяем позицию игрока
        auto [px, py] = game->getPlayer()->getPosition();
        if (newX == px && newY == py) {
            occupied = true;
        }

        // Проверяем других союзников
        if (!occupied) {
            for (const auto& ally : game->getAllies()) {
                if (ally.get() == this) continue;
                if (!ally->isAlive()) continue;
                
                auto [ax, ay] = ally->getPosition();
                if (newX == ax && newY == ay) {
                    occupied = true;
                    break;
                }
            }
        }

        // Проверяем врагов
        if (!occupied) {
            for (const auto& enemy : game->getEnemies()) {
                if (!enemy->isAlive()) continue;
                auto [ex, ey] = enemy->getPosition();
                if (newX == ex && newY == ey) {
                    occupied = true;
                    break;
                }
            }
        }

        if (occupied) {
            break;
        }

        positionX = newX;
        positionY = newY;
        
        // Пересчитываем расстояние до врага
        dx = enemyX - positionX;
        dy = enemyY - positionY;
        
        // Обновляем направление движения
        if (std::abs(dx) > std::abs(dy)) {
            moveX = (dx > 0) ? 1 : -1;
            moveY = 0;
        } else if (std::abs(dy) > 0) {
            moveY = (dy > 0) ? 1 : -1;
            moveX = 0;
        } else {
            break;
        }
    }
    
    // Проверяем, можем ли атаковать после движения
    int newDistance = std::max(std::abs(enemyX - positionX), std::abs(enemyY - positionY));
    if (newDistance <= 1 && nearestEnemy->isAlive()) {
        attack(nearestEnemy);
    }
}

/**
 * @brief Найти ближайшего врага или башню
 * @param game Указатель на игру
 * @return Пара: (указатель на врага, расстояние до него)
 * 
 * Использует чебышевскую метрику расстояния (максимум разниц координат).
 * Ищет среди обычных врагов и вражеских башен.
 */
std::pair<GameEntity*, int> Ally::findNearestEnemy(Game* game) {
    GameEntity* nearest = nullptr;
    int minDistance = INT_MAX;
    
    // Ищем среди обычных врагов
    for (auto& enemy : game->getEnemies()) {
        if (!enemy->isAlive()) continue;
        
        auto [ex, ey] = enemy->getPosition();
        int dx = std::abs(ex - positionX);
        int dy = std::abs(ey - positionY);
        int dist = std::max(dx, dy);
        
        if (dist < minDistance) {
            minDistance = dist;
            nearest = enemy.get();
        }
    }

    // Ищем среди башен
    for (auto& tower : game->getTowers()) {
        if (!tower->isAlive()) continue;
        
        auto [tx, ty] = tower->getPosition();
        int dx = std::abs(tx - positionX);
        int dy = std::abs(ty - positionY);
        int dist = std::max(dx, dy);
        
        if (dist < minDistance) {
            minDistance = dist;
            nearest = tower.get();
        }
    }

    return {nearest, minDistance};
}


