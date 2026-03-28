#pragma once
#include "ISpell.h"


class IAreaDamageSpell : public ISpell {
public:
    virtual double getAreaSize() const = 0;
};