#ifndef TRAP_H
#define TRAP_H

#include "GameEntity.h"

/**
 * @file Trap.h
 * @brief Класс ловушки на игровом поле
 * 
 * Ловушка устанавливается на поле и срабатывает
 * когда враг наступает на неё.
 */

/**
 * @class Trap
 * @brief Ловушка с уроном
 * 
 * После срабатывания ловушка исчезает.
 */
class Trap : public GameEntity {
private:
    bool activated; ///< Флаг активации ловушки
    
public:
    /**
     * @brief Конструктор ловушки
     * @param posX X-координата
     * @param posY Y-координата
     * @param trapDamage Урон ловушки
     */
    Trap(int posX, int posY, int trapDamage);
    
    /**
     * @brief Активировать ловушку
     * @return Урон, который наносит ловушка
     * 
     * После активации ловушка помечается как неживая.
     */
    int activate();
    
    /**
     * @brief Проверить, активирована ли ловушка
     * @return true если ловушка была активирована
     */
    bool isActivated() const { return activated; }
    
    /**
     * @brief Получить тип сущности
     * @return "Trap"
     */
    std::string getType() const override { return "Trap"; }
};

#endif // TRAP_H


