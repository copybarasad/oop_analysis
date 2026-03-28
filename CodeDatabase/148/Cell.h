#ifndef CELL_H
#define CELL_H

#include "GameTypes.h"

/**
 * @file Cell.h
 * @brief Класс клетки игрового поля
 * 
 * Представляет отдельную клетку на игровом поле с определенным типом
 * и свойствами проходимости.
 */

/**
 * @class Cell
 * @brief Класс представляющий одну клетку игрового поля
 * 
 * Отвечает за хранение типа клетки и определение ее свойств:
 * проходимость, специальные эффекты и т.д.
 * Не зависит от сущностей на клетке - это ответственность Field.
 */
class Cell {
private:
    CellType type;  ///< Тип клетки определяющий ее свойства
    
public:
    /**
     * @brief Конструктор клетки
     * @param cellType Тип создаваемой клетки (по умолчанию EMPTY)
     */
    Cell(CellType cellType = CellType::EMPTY) noexcept;
    
    // Базовые геттеры и сеттеры
    CellType getType() const noexcept { return type; }     ///< Получить тип клетки
    void setType(CellType newType) noexcept { type = newType; } ///< Установить тип клетки
    
    /**
     * @brief Проверить проходимость клетки
     * @return true если клетка проходима, иначе false
     * 
     * BLOCKED клетки непроходимы, все остальные - проходимы.
     */
    bool isPassable() const noexcept;
   
};

#endif // CELL_H

