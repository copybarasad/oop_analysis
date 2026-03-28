#include "EnemyTower.h"
#include "Game.h"
#include "GameEventSystem.h"
#include "GameEvent.h"
#include <cmath>

/**
 * @file EnemyTower.cpp
 * @brief Реализация класса вражеской башни
 */

/**
 * @brief Конструктор вражеской башни
 * @param posX X-координата башни
 * @param posY Y-координата башни
 * @param health Здоровье башни
 * @param damage Урон атаки
 * @param range Радиус атаки
 * @param cooldownTime Время перезарядки между атаками
 * 
 * Инициализирует башню с вызовом конструктора базового класса GameEntity.
 */
EnemyTower::EnemyTower(int posX, int posY, int health, int damage, int range, int cooldownTime)
    : GameEntity(health, damage, posX, posY), 
      attackRange(range), 
      cooldown(0), 
      maxCooldown(cooldownTime) {
}

/**
 * @brief Получить урон (переопределение с логированием)
 * @param amount Количество урона
 */
void EnemyTower::takeDamage(int amount) {
    int oldHealth = health;
    
    // Вызываем базовый метод
    GameEntity::takeDamage(amount);
    
    // Логируем урон башни
    auto [tx, ty] = getPosition();
    std::string towerDesc = "Tower at (" + std::to_string(tx) + "," + std::to_string(ty) + ")";
    DamageEvent event(EventType::ENEMY_DAMAGED, "Player", towerDesc, amount, health);
    GameEventSystem::getInstance().notifyEvent(event);
    
    // Если башня уничтожена
    if (health == 0 && oldHealth > 0) {
        EntityEvent deathEvent(EventType::TOWER_DESTROYED, "Tower", tx, ty, 
                               "Tower at (" + std::to_string(tx) + "," + std::to_string(ty) + ") destroyed!");
        GameEventSystem::getInstance().notifyEvent(deathEvent);
    }
}

/**
 * @brief Атаковать цель (переопределение с логированием)
 * @param target Указатель на цель
 */
void EnemyTower::attack(GameEntity* target) {
    if (!target || !target->isAlive()) return;
    
    int targetHealthBefore = target->getHealth();
    target->takeDamage(this->damage);
    int targetHealthAfter = target->getHealth();
    
    int actualDamage = targetHealthBefore - targetHealthAfter;
    
    // Логируем атаку башни
    auto [tx, ty] = getPosition();
    std::string towerDesc = "Tower at (" + std::to_string(tx) + "," + std::to_string(ty) + ")";
    DamageEvent event(EventType::ENEMY_ATTACK, towerDesc, "Player", actualDamage, targetHealthAfter);
    GameEventSystem::getInstance().notifyEvent(event);
}

/**
 * @brief Выполнить ход башни
 * @param game Указатель на игру
 * 
 * Башня каждый ход:
 * 1. Уменьшает перезарядку
 * 2. Проверяет наличие игрока в радиусе
 * 3. Атакует если радиус позволяет и перезарядка готова
 */
void EnemyTower::takeTurn(Game* game) {
    if (!game || !isAlive()) {
        return;
    }

    // Уменьшаем перезарядку
    if (cooldown > 0) {
        cooldown--;
        return;
    }

    // Проверяем, в радиусе ли игрок
    Player* player = game->getPlayer();
    auto [playerX, playerY] = player->getPosition();
    
    int dx = std::abs(playerX - positionX);
    int dy = std::abs(playerY - positionY);
    int distance = std::max(dx, dy);
    
    if (distance <= attackRange) {
        attack(player);
        cooldown = maxCooldown;
    }
}


