#pragma once
#include "ISpell.h"

class ITrapSpell : public ISpell {
public:
    virtual bool isValidPlacement(Field& field, int x, int y) = 0;
};