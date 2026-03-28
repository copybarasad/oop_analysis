#pragma once
#include "ISpell.h"


class IDirectDamageSpell : public ISpell {
public:
    virtual bool requiresTarget() const = 0;
};