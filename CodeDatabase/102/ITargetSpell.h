#pragma once
#include "ISpell.h"
#include "PositionEntity.h"

class ITargetSpell : public ISpell
{
public:
    virtual void SetTarget(PositionEntity* entity) = 0;
};