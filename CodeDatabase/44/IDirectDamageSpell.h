#pragma once
#include "Spell.h"
#include <string>

class Entity;

class IDirectDamageSpell : public Spell {
public:
    virtual int GetTarget(int available_size) const = 0;
};