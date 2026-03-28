#pragma once
#include "PositionEntity.h"
#include "../MoveResult.h"

class Movable : public PositionEntity
{
friend class MovableController;
protected:
    int _speed;

    virtual void OnMoveSuccess(Cell target) = 0;

    virtual void OnMoveFailure(MoveResult result) = 0;
public:
    Movable(Cell position, int speed);

    int GetSpeed() const;
};