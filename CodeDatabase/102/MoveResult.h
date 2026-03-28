#pragma once
#include "Entities/PositionEntity.h"

struct MoveResult
{
    PositionEntity* _collision{};
    Cell _destination;
    bool _isWallCollision{};
};