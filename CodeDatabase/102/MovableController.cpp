#include "../../Headers/Controllers/MovableController.h"

MovableController::MovableController(MovableContainer* container, FieldContainer* fieldContainer) : _container{ container }, _fieldContainer{ fieldContainer }
{
}

MoveResult MovableController::Move(Movable* movable, Direction direction) const
{
    MoveResult result = GetMoving(movable, direction);
    movable->_position = result._destination;
    if (result._collision == nullptr && !result._isWallCollision)
        movable->OnMoveSuccess(result._destination);
    else
        movable->OnMoveFailure(result);
    return result;
}

MoveResult MovableController::GetMoving(const Movable* movable, Direction direction) const
{
    MoveResult result;
    Cell target = GetPosition(movable, direction);
    Cell currentPosition = movable->GetPosition();
    target = _fieldContainer->GetCell(target._x, target._y);
    if (target == Cell())
    {
        result._destination = currentPosition;
        result._isWallCollision = true;
    }
    else
    {
        PositionEntity* entity = _fieldContainer->GetEntity(target);
        if (entity != nullptr)
        {
            result._collision = entity;
            result._destination = currentPosition;
        }
        else
            result._destination = target;
    }
    return result;
}

Cell MovableController::GetPosition(const Movable* movable, Direction direction) const
{
    Cell position = movable->GetPosition();
    switch (direction)
    {
    case Direction::Left:
        return Cell(position._x, position._y - movable->GetSpeed());
    case Direction::Up:
        return Cell(position._x - movable->GetSpeed(), position._y);
    case Direction::Right:
        return Cell(position._x, position._y + movable->GetSpeed());
    case Direction::Down:
        return Cell(position._x + movable->GetSpeed(), position._y);
    }
    return Cell();
}

bool MovableController::AddEntity(PositionEntity* entity, Movable* movable)
{
    return _container->AddEntity(entity, movable);
}

bool MovableController::RemoveEntity(PositionEntity* entity)
{
    return _container->RemoveEntity(entity);
}