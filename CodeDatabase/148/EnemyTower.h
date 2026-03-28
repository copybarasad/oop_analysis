#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "GameEntity.h"
#include "GameTypes.h"

class Game;

/**
 * @file EnemyTower.h
 * @brief Класс вражеской башни
 * 
 * Башня атакует игрока на расстоянии, но не каждый ход,
 * а с задержкой (перезарядкой).
 */

/**
 * @class EnemyTower
 * @brief Вражеская башня с перезарядкой
 * 
 * Характеристики:
 * - Здоровье: 80
 * - Урон: 20
 * - Радиус атаки: 5
 * - Перезарядка: 2 хода
 */
class EnemyTower : public GameEntity {
private:
    int attackRange;      ///< Радиус атаки башни
    int cooldown;         ///< Текущая перезарядка
    int maxCooldown;      ///< Максимальная перезарядка
    
public:
    /**
     * @brief Конструктор вражеской башни
     * @param posX X-координата
     * @param posY Y-координата
     * @param health Здоровье (по умолчанию 80)
     * @param damage Урон (по умолчанию 20)
     * @param range Радиус атаки (по умолчанию 5)
     * @param cooldownTime Время перезарядки (по умолчанию 2)
     */
    EnemyTower(int posX, int posY, int health = TOWER_START_HEALTH, int damage = TOWER_ATTACK_DAMAGE, 
               int range = TOWER_ATTACK_RANGE, int cooldownTime = 2);
    
    /**
     * @brief Выполнить ход башни
     * @param game Указатель на игру
     * 
     * Башня проверяет, может ли атаковать (перезарядка),
     * и если да - атакует игрока если он в радиусе.
     */
    void takeTurn(Game* game);
    
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
     * @brief Получить тип сущности
     * @return "Tower"
     */
    std::string getType() const override { return "Tower"; }
    
    /**
     * @brief Получить текущую перезарядку
     * @return Количество ходов до следующей атаки
     */
    int getCooldown() const { return cooldown; }
    
    /**
     * @brief Получить радиус атаки
     * @return Радиус атаки
     */
    int getAttackRange() const { return attackRange; }
};

#endif // ENEMYTOWER_H


