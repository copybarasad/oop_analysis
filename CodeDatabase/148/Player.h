#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "GameEntity.h"
#include "GameTypes.h"

/**
 * @file Player.h
 * @brief Класс игрока
 * 
 * Наследуется от GameEntity и добавляет специфичные
 * для игрока функции: смена режима атаки, счёт, убийства.
 */

/**
 * @class Player
 * @brief Игрок
 * 
 * Может переключаться между режимами атаки, зарабатывать очки,
 * покупать заклинания.
 */
class Player : public GameEntity {
private:
    AttackMode attackMode;  ///< Режим атаки (MELEE или RANGED)
    int score;              ///< Очки игрока
    int enemiesKilled;      ///< Количество убитых врагов
    int maxHealth;          ///< Максимальное здоровье игрока
    
public:
    /**
     * @brief Конструктор игрока
     * @param startX X-координата
     * @param startY Y-координата
     * @param health Здоровье (по умолчанию PLAYER_START_HEALTH)
     * @param damage Урон (по умолчанию PLAYER_START_DAMAGE)
     * 
     * Создаёт игрока в режиме MELEE.
     */
    Player(int startX, int startY, int health = PLAYER_START_HEALTH, int damage = PLAYER_START_DAMAGE);
    
    /**
     * @brief Переключить режим атаки
     * 
     * Переключает между MELEE и RANGED.
     * - MELEE: полный урон, ближний бой
     * - RANGED: половинный урон, дальняя атака
     */
    void switchAttackMode();
    
    /**
     * @brief Получить текущий режим атаки
     * @return MELEE или RANGED
     */
    AttackMode getAttackMode() const {
        return attackMode;
    }
    /**
 * @brief Переместить игрока (override с логированием)
 * @param newX Новая X координата
 * @param newY Новая Y координата
 */
void move(int newX, int newY) override;

/**
 * @brief Получить урон (override с логированием)
 * @param damageAmount Количество урона
 */
void takeDamage(int damageAmount) override;

/**
 * @brief Атаковать цель (override с логированием)
 * @param target Указатель на цель
 */
void attack(GameEntity* target);

    /**
     * @brief Установить режим атаки
     * @param mode Новый режим атаки
     */
    void setAttackMode(AttackMode mode) {
        attackMode = mode;
    }
    
    /**
     * @brief Получить урон с учётом режима атаки
     * @return Урон
     * 
     * Override из GameEntity.
     * - MELEE: damage * 1.0
     * - RANGED: damage * 0.5
     */
    int getDamage() const noexcept override;
    
    /**
     * @brief Получить текущие очки
     * @return Очки
     */
    int getScore() const {
        return score;
    }
    
    /**
     * @brief Установить очки
     * @param newScore Новое значение очков
     */
    void setScore(int newScore) {
        score = newScore;
    }
    
    /**
     * @brief Добавить очки
     * @param points Количество очков для добавления
     */
    void addScore(int points) {
        score += points;
        std::cout << "Gained " << points << " points! Total score: " << score << "\n";
    }
    
    /**
     * @brief Проверить, может ли игрок купить заклинание
     * @return true если есть 10+ очков, false иначе
     */
    bool canBuySpell() const {
        return score >= SPELL_COST;
    }
    
    /**
     * @brief Купить заклинание за очки
     * @return true если куплено, false если не хватает очков
     * 
     * Стоимость: 10 очков.
     */
    bool buySpell() {
        if (score >= 10) {
            score -= 10;
            std::cout << "Bought spell for 10 points! Remaining score: " << score << "\n";
            return true;
        }
        std::cout << "Not enough points! Need 10, have " << score << "\n";
        return false;
    }
    
    /**
     * @brief Получить количество убитых врагов
     * @return Количество убийств
     */
    int getEnemiesKilled() const {
        return enemiesKilled;
    }
    
    /**
     * @brief Установить количество убийств
     * @param kills Новое количество
     */
    void setEnemiesKilled(int kills) {
        enemiesKilled = kills;
    }
    
    /**
     * @brief Увеличить счётчик убийств на 1
     */
    void incrementKillCount() {
        enemiesKilled++;
    }
    
    /**
     * @brief Получить максимальное здоровье
     * @return Максимальное здоровье
     */
    int getMaxHealth() const {
        return maxHealth;
    }
    
    /**
     * @brief Установить максимальное здоровье
     * @param newMaxHealth Новое максимальное здоровье
     */
    void setMaxHealth(int newMaxHealth) {
        maxHealth = newMaxHealth;
    }
    
    /**
     * @brief Установить урон
     * @param newDamage Новый урон
     */
    void setDamage(int newDamage) {
        damage = newDamage;
    }
    
    /**
     * @brief Получить тип сущности
     * @return "Player"
     * 
     * Override из GameEntity.
     */
    std::string getType() const override {
        return "Player";
    }
};

#endif // PLAYER_H


