#include "GameUtils.h"

int GameUtils::clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

char GameUtils::directionToCommand(MoveDirection dir) {
    switch (dir) {
        case MoveDirection::North: return 'w';
        case MoveDirection::South: return 's';
        case MoveDirection::West: return 'a';
        case MoveDirection::East: return 'd';
        default: return ' ';
    }
}

MoveDirection GameUtils::commandToDirection(char command) {
    switch (command) {
        case 'w': return MoveDirection::North;
        case 's': return MoveDirection::South;
        case 'a': return MoveDirection::West;
        case 'd': return MoveDirection::East;
        default: return MoveDirection::North;
    }
}