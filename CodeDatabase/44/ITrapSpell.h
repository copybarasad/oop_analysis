#pragma once
#include "Spell.h"
#include "Field.h"

class ITrapSpell : public Spell {
public:
    virtual std::pair<int, int> GetCoordsForTrap(Field& field) const = 0;  
};
