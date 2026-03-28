#include "direction.h"

int directionToDx(Direction direction) {
    switch (direction) {
        case Direction::LEFT: return -1;
        case Direction::RIGHT: return 1;
        default: return 0;
    }
}

int directionToDy(Direction direction) {
    switch (direction) {
        case Direction::UP: return -1;
        case Direction::DOWN: return 1;
        default: return 0;
    }
}

int normalizeComponent(int value) {
    return (value > 0) ? 1 : (value < 0) ? -1 : 0;
}