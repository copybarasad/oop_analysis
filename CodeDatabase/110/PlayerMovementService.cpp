#include "PlayerMovementService.hpp"

MovementService::MoveResult PlayerMovementService::move (Field& field, Cell& currentCell, size_t x, size_t y) {
    MoveResult result;
    if (isBlocked(field, x, y)) {
        result.success = false;
        return result;
    }
    
    Cell& newCell = field.getCell(x, y);

    if (checkCloseFight(result, currentCell, newCell)) {
        return result;
    }
    
    result.newCell = &newCell;
    swapCells(currentCell, newCell);

    checkRangedFight(field, result, newCell);

    result.slowTriggered = isSlow(field, x, y);

    return result;
}

bool PlayerMovementService::checkCloseFight (MoveResult& result, Cell& currentCell, Cell& goalCell) const {
    if (currentCell.getEntityAs<Player>().getMode() == CombatMode::CLOSE && !goalCell.isEmpty()) {
        result.fight = true;
        result.opponent = &goalCell;
        return true;
    }
    return false;
}

bool PlayerMovementService::checkRangedFight (Field& field, MoveResult& result, Cell& newCell) const {
    if (newCell.getEntityAs<Player>().getMode() != CombatMode::RANGED) {
        return false;
    }
    int x = newCell.getX();
    int y = newCell.getY();
    int distance = newCell.getEntityAs<Player>().getWeapon()->getRange();
    
    if (checkDirection(field, x + distance, y)) {
        result.opponent = &field.getCell(x + distance, y);
        result.fight = true;
        return true;
    }
    if (checkDirection(field, x - distance, y)) {
        result.opponent = &field.getCell(x + distance, y);
        result.fight = true;
        return true;
    }
    if (checkDirection(field, x, y + distance)) {
        result.opponent = &field.getCell(x + distance, y);
        result.fight = true;
        return true;
    }
    if (checkDirection(field, x, y - distance)) {
        result.opponent = &field.getCell(x + distance, y);
        result.fight = true;
        return true;
    }

    return false;
}

bool PlayerMovementService::checkDirection (Field& field, int x, int y) const {
    return (moveIsAble(field, x, y) && !field.getCell(x, y).isEmpty());
}