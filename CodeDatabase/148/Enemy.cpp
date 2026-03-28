#include "Enemy.h"
#include "GameTypes.h"
#include "GameEventSystem.h"
#include "GameEvent.h"
#include <cstdlib>
#include <cmath>

/**
 * @file Enemy.cpp
 * @brief Реализация методов класса Enemy
 */

/**
 * @brief Конструктор врага
 * @param startX X-координата
 * @param startY Y-координата
 * @param health Здоровье (по умолчанию ENEMY_START_HEALTH = 50)
 * @param damage Урон (по умолчанию ENEMY_START_DAMAGE = 15)
 * 
 * Инициализирует врага со стандартными характеристиками:
 * - Здоровье: ENEMY_START_HEALTH = 50
 * - Урон: ENEMY_START_DAMAGE = 15
 * Враги слабее игрока но представляют угрозу в группе.
 */
Enemy::Enemy(int startX, int startY, int health, int damage) 
    : GameEntity(health, damage, startX, startY) {
}

/**
 * @brief Расчет следующего шага к цели
 * 
 * Простой алгоритм преследования: двигается по оси с наибольшей разницей.
 * Это создает естественное поведение преследования без сложных вычислений.
 * 
 * Пример: 
 * - Если разница по X больше чем по Y → двигается по X
 * - Если разница по Y больше чем по X → двигается по Y
 * - Если на одной линии с игроком → стоит на месте
 * 
 * @param targetX X-координата цели
 * @param targetY Y-координата цели  
 * @return Координаты следующей позиции
 */
std::pair<int, int> Enemy::desiredStepTowards(int targetX, int targetY) const {
    auto [currentX, currentY] = getPosition();
    
    int dx = targetX - currentX;
    int dy = targetY - currentY;

    int stepX = 0;
    int stepY = 0;

    // Выбираем ось с наибольшей разницей координат
    if (std::abs(dx) > std::abs(dy)) {
        stepX = (dx > 0) ? 1 : -1;  // Двигаемся по X
    } else if (dy != 0) {
        stepY = (dy > 0) ? 1 : -1;  // Двигаемся по Y
    }
    // Если dx и dy равны 0, остаемся на месте (stepX=0, stepY=0)

    return {currentX + stepX, currentY + stepY};
}

/**
 * @brief Получить урон (переопределение с логированием)
 * @param amount Количество урона
 */
void Enemy::takeDamage(int amount) {
    int oldHealth = health;
    
    // Вызываем базовый метод
    GameEntity::takeDamage(amount);
    
    // Логируем урон врага
    auto [ex, ey] = getPosition();
    std::string enemyDesc = "Enemy at (" + std::to_string(ex) + "," + std::to_string(ey) + ")";
    DamageEvent event(EventType::ENEMY_DAMAGED, "Player", enemyDesc, amount, health);
    GameEventSystem::getInstance().notifyEvent(event);
    
    // Если враг умер
    if (health == 0 && oldHealth > 0) {
        EntityEvent deathEvent(EventType::ENEMY_DIED, "Enemy", ex, ey, 
                               "Enemy at (" + std::to_string(ex) + "," + std::to_string(ey) + ") died!");
        GameEventSystem::getInstance().notifyEvent(deathEvent);
    }
}

/**
 * @brief Атаковать цель (переопределение с логированием)
 * @param target Указатель на цель
 */
void Enemy::attack(GameEntity* target) {
    if (!target || !target->isAlive()) return;
    
    int targetHealthBefore = target->getHealth();
    target->takeDamage(this->damage);
    int targetHealthAfter = target->getHealth();
    
    int actualDamage = targetHealthBefore - targetHealthAfter;
    
    // Логируем атаку врага
    auto [ex, ey] = getPosition();
    std::string enemyDesc = "Enemy at (" + std::to_string(ex) + "," + std::to_string(ey) + ")";
    DamageEvent event(EventType::ENEMY_ATTACK, enemyDesc, "Player", actualDamage, targetHealthAfter);
    GameEventSystem::getInstance().notifyEvent(event);
}


