#ifndef TRAP_SPELL_INTERFACE_H
#define TRAP_SPELL_INTERFACE_H

#include "spell_interface.h"

class ITrapSpell : public ISpell
{
public:
    virtual ~ITrapSpell() = default;
    virtual int get_damage() const = 0;
};

#endif