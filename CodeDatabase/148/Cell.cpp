#include "Cell.h"

/**
 * @file Cell.cpp
 * @brief Реализация методов класса Cell
 */

/**
 * @brief Конструктор клетки
 * 
 * Инициализирует клетку указанным типом. Не вызывает исключений
 * для гарантированной безопасности при создании больших сеток.
 */
Cell::Cell(CellType cellType) noexcept : type(cellType) {
}

/**
 * @brief Определение проходимости клетки
 * 
 * На текущий момент только BLOCKED клетки являются непроходимыми.
 * В будущем можно расширить логику для SLOW клеток или других типов.
 * 
 * @return true если клетка проходима, false если непроходима
 */
bool Cell::isPassable() const noexcept {
    return type != CellType::BLOCKED;
}


