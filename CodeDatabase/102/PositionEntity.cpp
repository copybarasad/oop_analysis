#include "../../Headers/Entities/PositionEntity.h"

PositionEntity::PositionEntity(Cell position) : _position{ position }
{
}

Cell PositionEntity::GetPosition() const
{
	return _position;
}