#pragma once
#include "InterfaceSpell.hpp"


class InterfaceDirectDamageSpell : public virtual InterfaceSpell {
public:
    virtual int getDamage() const = 0;
    virtual int getRadius() const = 0;
};
