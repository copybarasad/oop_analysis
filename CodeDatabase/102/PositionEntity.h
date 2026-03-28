#pragma once
#include "../Units.h"

class PositionEntity
{
protected:
    Cell _position;
public:
    virtual ~PositionEntity() = default;

    explicit PositionEntity(Cell position);

    Cell GetPosition() const;
};