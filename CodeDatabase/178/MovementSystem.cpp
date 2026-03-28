#include "MovementSystem.h"

MovementSystem::MovementSystem(GameField& gameField) : field(gameField) {}

Position MovementSystem::calculateNewPosition(const Position &current, const Position &direction) const {
    return Position(current.x + direction.x, current.y + direction.y);
}

bool MovementSystem::isValidMove(const Position &newPosition) const {
    return field.isValidPosition(newPosition) && field.isPositionPassable(newPosition);
}

void MovementSystem::movePlayer(Player &player, const Position &newPosition) {
    player.move(newPosition);
    player.addScore(1);
}

void MovementSystem::moveEnemy(Enemy &enemy, const Position &newPosition) {
    enemy.move(newPosition);
}