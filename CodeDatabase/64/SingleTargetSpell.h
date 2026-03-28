#ifndef SINGLE_TARGET_SPELL_H
#define SINGLE_TARGET_SPELL_H

#include "Spell.h"

class SingleTargetSpell : public Spell {
private:
    int damage;

public:
    SingleTargetSpell(const std::string& name, int damage, int range);
    int getDamage() const { return damage; }
};

#endif