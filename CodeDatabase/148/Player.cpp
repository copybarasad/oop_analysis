#include "Player.h"
#include "GameTypes.h"
#include "GameEventSystem.h"
#include "GameEvent.h"

/**
 * @file Player.cpp
 * @brief Реализация класса Player
 */

/**
 * @brief Конструктор Player
 * 
 * Инициализирует игрока с заданными параметрами.
 * @param startX X-координата
 * @param startY Y-координата
 * @param health Здоровье (по умолчанию PLAYER_START_HEALTH)
 * @param damage Урон (по умолчанию PLAYER_START_DAMAGE)
 * 
 * Вызывает конструктор GameEntity.
 */
Player::Player(int startX, int startY, int health, int damage)
    : GameEntity(health, damage, startX, startY),
      attackMode(AttackMode::MELEE),
      score(0),
      enemiesKilled(0),
      maxHealth(health)
{
}

/**
 * @brief Переключить режим атаки
 * 
 * Меняет MELEE на RANGED и наоборот.
 */
void Player::switchAttackMode() {
    if (attackMode == AttackMode::MELEE) {
        attackMode = AttackMode::RANGED;
    } else {
        attackMode = AttackMode::MELEE;
    }
    
    // Логируем смену режима
    std::string mode = (attackMode == AttackMode::MELEE) ? "MELEE" : "RANGED";
    GameEvent event(EventType::ATTACK_MODE_SWITCHED, "Attack mode switched to " + mode);
    GameEventSystem::getInstance().notifyEvent(event);
}

/**
 * @brief Получить урон с учётом режима атаки
 * @return Урон
 * 
 * - MELEE: damage * 1.0 (например, 40 * 1.0 = 40)
 * - RANGED: damage * 0.5 (например, 40 * 0.5 = 20)
 */
int Player::getDamage() const noexcept {
    if (attackMode == AttackMode::MELEE) {
        return static_cast<int>(damage * MELEE_DAMAGE_MULTIPLIER);
    }
    return static_cast<int>(damage * RANGED_DAMAGE_MULTIPLIER);
}

/**
 * @brief Переместить игрока (переопределение с логированием)
 * @param newX Новая X координата
 * @param newY Новая Y координата
 */
void Player::move(int newX, int newY) {
    // Сохраняем старую позицию для события
    int oldX = positionX;
    int oldY = positionY;
    
    // Вызываем базовый метод для обновления позиции
    GameEntity::move(newX, newY);
    
    // Логируем событие движения
    MovementEvent event("Player", oldX, oldY, newX, newY);
    GameEventSystem::getInstance().notifyEvent(event);
}

/**
 * @brief Получить урон (переопределение с логированием)
 * @param damageAmount Количество урона
 */
void Player::takeDamage(int damageAmount) {
    int oldHealth = health;
    GameEntity::takeDamage(damageAmount);
    
    // УДАЛИЛИ ЛОГ - Enemy::attack() и Tower::attack() уже сами логируют!
    
    if (health == 0 && oldHealth > 0) {
        GameEvent deathEvent(EventType::PLAYER_DAMAGED, "Player died!");
        GameEventSystem::getInstance().notifyEvent(deathEvent);
    }
}


/**
 * @brief Атаковать цель (переопределение с логированием)
 * @param target Указатель на цель
 */
void Player::attack(GameEntity* target) {
    if (!target || !target->isAlive()) return;
    
    int targetHealthBefore = target->getHealth();
    int playerDamage = getDamage();  // Используем переопределённый getDamage()
    target->takeDamage(playerDamage);
    int targetHealthAfter = target->getHealth();
    
    int actualDamage = targetHealthBefore - targetHealthAfter;
    
    // Логируем атаку игрока
    std::string attackType = (attackMode == AttackMode::MELEE) ? "MELEE" : "RANGED";
    DamageEvent event(EventType::PLAYER_ATTACK, 
                     "Player (" + attackType + ")", 
                     "Enemy", 
                     actualDamage, 
                     targetHealthAfter);
    GameEventSystem::getInstance().notifyEvent(event);
}

/**
 * @brief Установить урон (для загрузки игры / улучшений)
 * @param newDamage Новый базовый урон
 */


