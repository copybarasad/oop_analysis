#include "AllyMovementService.hpp"

MovementService::MoveResult AllyMovementService::move (Field& field, Cell& currentCell, size_t x, size_t y) {
    MoveResult result;
    std::pair<int, int> currentPosition = std::make_pair(currentCell.getX(), currentCell.getY());
    auto step = calculateAllyMove(field, currentPosition, std::make_pair(x, y));
    auto newPosition = std::make_pair(currentCell.getX() + step.first, currentCell.getY() + step.second);

    if (isBlocked(field, currentPosition.first, currentPosition.second)) {
        currentPosition = findAlternativeMove(field, currentPosition);
        if (currentPosition.first == -1) {
            result.success = false;
            return result;
        }
    }

    Cell& newCell = field.getCell(newPosition.first, newPosition.second);

    if (!newCell.isEmpty() && newCell.isEnemy()) {
        result.fight = true;
        result.opponent = &newCell;
        return result;
    }

    result.newCell = &newCell;
    swapCells(currentCell, newCell);

    result.slowTriggered = isSlow(field, x, y);

    return result;
}

std::pair<int, int> AllyMovementService::calculateAllyMove (Field& field, std::pair<size_t, size_t> allyPosition, std::pair<size_t, size_t> playerPosition) const {
    auto enemyPosition = findEnemy (field, allyPosition);
    std::pair<size_t, size_t> target = playerPosition;
    if (enemyPosition.first != -1) {
        target = enemyPosition;
    }

    int x_dist = allyPosition.first - target.first;
    int y_dist = allyPosition.second - target.second;

    if (std::abs(x_dist) >= std::abs(y_dist)) {
        int x = (x_dist > 0) ? 1 : (x_dist < 0) ? -1 : 0;
        return {x, 0};
    } else {
        int y = (y_dist > 0) ? 1 : (y_dist < 0) ? -1 : 0;
        return {0, y};
    }
}

std::pair<int, int> AllyMovementService::findEnemy (Field& field, std::pair<size_t, size_t> allyPosition) const {
    for (int y = allyPosition.second - 5; y <= (int)allyPosition.second + 5; y++) {
        for (int x = allyPosition.first - 5; x <= (int)allyPosition.first + 5; x++) {
            if (checkEnemy(field, x, y)) {
                return {x, y};
            }
        }
    }

    return {-1, -1};
}

bool AllyMovementService::checkEnemy (Field& field, int x, int y) const {
    return x >= 0 && x < (int)field.getWidth() && y >= 0 && y < (int)field.getHeight()
            && !field.getCell(x, y).isEmpty() && field.getCell(x, y).isEnemy();
}

std::pair<int, int>  AllyMovementService::findAlternativeMove (const Field& field, std::pair<size_t, size_t> enemyPosition) const {
    std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (const auto& dir : directions) {
        int x = enemyPosition.first + dir.first;
        int y = enemyPosition.second + dir.second;
        size_t newX = x >= 0 ? (x < (int)field.getWidth() ? x : field.getWidth()) : 0;
        size_t newY = y >= 0 ? (y < (int)field.getHeight() ? y : field.getHeight()) : 0;
        
        if (moveIsAble(field, newX, newY) && !isBlocked(field, newX, newY) && field.getCell(newX, newY).isEmpty()) {
            return dir;
        }
    }
    return std::make_pair(0, 0);
}