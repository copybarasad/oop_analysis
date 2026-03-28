#pragma once
#include "Position.h"

enum class Direction {
    UP,
    DOWN, 
    LEFT,
    RIGHT,
    NONE
};

class DirectionHelper {
public:
    static Direction convertToDirection(char input);
    static Position getPositionOffset(Direction direction);
};