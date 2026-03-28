#include "MovementService.hpp"

bool MovementService::moveIsAble (const Field& field, int x, int y) const {
    return (0 <= x and x < (int)field.getWidth()) and (0 <= y and y < (int)field.getHeight());
}

bool MovementService::isSlow (const Field& field, size_t x, size_t y) const {
    return field.getCell(x, y).getType() == CellType::SLOWED;
}

bool MovementService::isBlocked (const Field& field, size_t x, size_t y) const {
    return field.getCell(x, y).getType() == CellType::BLOCKED;
}

void MovementService::swapCells (Cell& current, Cell& next) {
    next.setEntity(current.getEntity());
    current.clear();
}