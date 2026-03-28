#include "../../Headers/Entities/Movable.h"

Movable::Movable(Cell position, int speed) : PositionEntity(position), _speed{ speed }
{
}

int Movable::GetSpeed() const
{
	return _speed;
}