#pragma once
#include "../Abstractions/PositionEntityContainer.h"
#include "../Entities/Movable.h"

class MovableContainer : public PositionEntityContainer<Movable>
{
};