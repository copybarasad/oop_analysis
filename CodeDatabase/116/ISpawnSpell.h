#pragma once
#include "ISpell.h"

class SpawnSpellContext;

class ISpawnSpell : public ISpell{
public:
    ~ISpawnSpell() = default;
    virtual void apply(SpawnSpellContext& context) = 0;
};