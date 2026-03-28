#pragma once
#include "Spell.h"
#include <vector>

class Enemy;

class IAreaDamageSpell : public Spell {
public:
    virtual std::pair<int, int> GetAreaTopLeft(int field_width, int field_height) const = 0; 
};
