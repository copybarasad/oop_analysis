#pragma once
#include "MoveDirection.h"

namespace GameUtils {
    int clamp(int value, int min, int max);
    char directionToCommand(MoveDirection dir);
    MoveDirection commandToDirection(char command);
}