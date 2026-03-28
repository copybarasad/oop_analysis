#include "MovementValidator.h"
#include "GameField.h"
#include "Enemy.h"
#include "Player.h"
#include <cmath>


bool MovementValidator::isValidPosition(const Position& position, const GameField& field) {
    return position.x >= 0 && position.x < field.getWidth() &&
           position.y >= 0 && position.y < field.getHeight();
}

bool MovementValidator::isCellPassable(const Position& position, const GameField& field) {
    return isValidPosition(position, field) && 
           field.isCellPassable(position);
}

bool MovementValidator::hasEnemyAtPosition(const Position& position, const std::vector<Enemy>& enemies) {
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPosition() == position) {
            return true;
        }
    }
    return false;
}

bool MovementValidator::hasPlayerAtPosition(const Position& position, const Player& player) {
    return player.getPosition() == position;
}

bool MovementValidator::arePositionsAdjacent(const Position& pos1, const Position& pos2) {
    int deltaX = std::abs(pos1.x - pos2.x);
    int deltaY = std::abs(pos1.y - pos2.y);
    return (deltaX == 1 && deltaY == 0) || (deltaX == 0 && deltaY == 1);
}