#include "Direction.h"

Direction DirectionHelper::convertToDirection(char input) {
    switch (input) {
        case 'w': case 'W': return Direction::UP;
        case 's': case 'S': return Direction::DOWN;
        case 'a': case 'A': return Direction::LEFT;
        case 'd': case 'D': return Direction::RIGHT;
        default: return Direction::NONE;
    }
}

Position DirectionHelper::getPositionOffset(Direction direction) {
    switch (direction) {
        case Direction::UP:    return Position(0, -1);
        case Direction::DOWN:  return Position(0, 1);
        case Direction::LEFT:  return Position(-1, 0);
        case Direction::RIGHT: return Position(1, 0);
        default: return Position(0, 0);
    }
}