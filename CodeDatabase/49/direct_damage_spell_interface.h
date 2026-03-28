#ifndef DIRECT_DAMAGE_SPELL_INTERFACE_H
#define DIRECT_DAMAGE_SPELL_INTERFACE_H

#include "spell_interface.h"

class IDirectDamageSpell : public ISpell
{
public:
    virtual ~IDirectDamageSpell() = default;
    virtual int get_damage() const = 0;
};

#endif