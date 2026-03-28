#pragma once
#include "../Containers/FieldContainer.h"
#include "../Containers/MovableContainer.h"
#include "../MoveResult.h"

class MovableController
{
    MovableContainer* _container;
    FieldContainer* _fieldContainer;
public:
    MovableController(MovableContainer* container, FieldContainer* fieldContainer);

    MoveResult Move(Movable* movable, Direction direction) const;

    MoveResult GetMoving(const Movable* movable, Direction direction) const;

    Cell GetPosition(const Movable* movable, Direction direction) const;

    bool AddEntity(PositionEntity* entity, Movable* movable);

    bool RemoveEntity(PositionEntity* entity);
};