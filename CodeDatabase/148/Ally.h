#ifndef ALLY_H
#define ALLY_H

#include "GameEntity.h"

class Game;

/**
 * @file Ally.h
 * @brief Класс союзника игрока
 * 
 * Союзник может быть призван игроком и действует автономно,
 * атакуя ближайших врагов.
 */

/**
 * @class Ally
 * @brief Союзник игрока
 * 
 * Характеристики:
 * - Здоровье: 50
 * - Урон: 15
 * - Дальность движения: 2 клетки за ход
 */
class Ally : public GameEntity {
private:
    int moveRange; ///< Дальность движения за ход
    
    /**
     * @brief Найти ближайшего врага
     * @param game Указатель на игру
     * @return Пара: указатель на врага и расстояние до него
     */
    std::pair<GameEntity*, int> findNearestEnemy(Game* game);
    
public:
    /**
     * @brief Конструктор союзника
     * @param posX X-координата
     * @param posY Y-координата
     * @param health Здоровье (по умолчанию 50)
     * @param damage Урон (по умолчанию 15)
     * @param moveRange Дальность движения (по умолчанию 2)
     */
    Ally(int posX, int posY, int health = 50, int damage = 15, int moveRange = 2);
    
    /**
     * @brief Выполнить ход союзника
     * @param game Указатель на игру
     * 
     * Союзник ищет ближайшего врага, движется к нему
     * и атакует если находится в радиусе атаки.
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
     * @return "Ally"
     */
    std::string getType() const override { return "Ally"; }
    
    /**
     * @brief Получить дальность движения
     * @return Дальность движения за ход
     */
    int getMoveRange() const { return moveRange; }
};

#endif // ALLY_H


