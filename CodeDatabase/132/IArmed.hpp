#pragma once

#include "weapon.hpp"
#include "IEntity.hpp"

class IArmed {
public:
    virtual void change_weapon() = 0;
    virtual Weapon get_current_weapon() const = 0;
};