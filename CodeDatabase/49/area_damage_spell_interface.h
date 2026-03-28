#ifndef AREA_DAMAGE_SPELL_INTERFACE_H
#define AREA_DAMAGE_SPELL_INTERFACE_H

#include "spell_interface.h"

class IAreaDamageSpell : public ISpell
{
public:
    virtual ~IAreaDamageSpell() = default;
    virtual int get_damage() const = 0;
    virtual int get_area_size() const = 0;
};

#endif