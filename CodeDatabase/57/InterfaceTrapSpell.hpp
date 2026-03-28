#pragma once
#include "InterfaceSpell.hpp"

class InterfaceTrapSpell : public virtual InterfaceSpell {
public:
    virtual int getTrapDamage() const = 0;
};
