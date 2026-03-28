#ifndef ENEMY_H
#define ENEMY_H

#include "GameEntity.h"
#include "GameTypes.h"

/**
 * @file Enemy.h
 * @brief Класс врага
 * 
 * Враг двигается к игроку и атакует при соприкосновении.
 */

/**
 * @class Enemy
 * @brief Враг игрока
 * 
 * Характеристики:
 * - Здоровье: 50 (берётся из ENEMY_START_HEALTH)
 * - Урон: 15 (берётся из ENEMY_START_DAMAGE)
 */
class Enemy : public GameEntity {
public:
    /**
     * @brief Конструктор врага
     * @param startX X-координата
     * @param startY Y-координата
     * @param health Здоровье (по умолчанию ENEMY_START_HEALTH = 50)
     * @param damage Урон (по умолчанию ENEMY_START_DAMAGE = 15)
     */
    Enemy(int startX, int startY, int health = ENEMY_START_HEALTH, int damage = ENEMY_START_DAMAGE);
    
    /**
     * @brief Вычислить желаемый шаг к цели
     * @param targetX X-координата цели
     * @param targetY Y-координата цели
     * @return Пара (dx, dy) - направление движения
     * 
     * Враг движется в сторону цели по прямой.
     */
    std::pair<int, int> desiredStepTowards(int targetX, int targetY) const;
    
    /**
     * @brief Получить урон (override с логированием)
     * @param amount Количество урона
     */
    void takeDamage(int amount) override;
    
    /**
     * @brief Атаковать цель (override с логированием)
     * @param target Указатель на цель
     */
    void attack(GameEntity* target);
    
    /**
     * @brief Установить урон
     * @param newDamage Новый урон
     */
    void setDamage(int newDamage) {
        damage = newDamage;
    }
    
    /**
     * @brief Получить тип сущности
     * @return "Enemy"
     */
    std::string getType() const override {
        return "Enemy";
    }
};

#endif // ENEMY_H


