#pragma once
#include "../Units.h"

class ISpell
{
public:
    virtual void Start(Cell position) = 0;

    virtual ~ISpell() = default;
};