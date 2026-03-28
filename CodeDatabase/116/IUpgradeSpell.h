#pragma once
#include "ISpell.h"

class UpgradeSpellContext;

class IUpgradeSpell : public ISpell {
public:
    virtual ~IUpgradeSpell() = default;
    virtual void apply(UpgradeSpellContext& context) = 0;
};