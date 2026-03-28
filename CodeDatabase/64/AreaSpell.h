#ifndef AREA_SPELL_H
#define AREA_SPELL_H

#include "Spell.h"

class AreaSpell : public Spell {
private:
    int damage;
    int areaSize;

public:
    AreaSpell(const std::string& name, int damage, int range, int areaSize = 2);
    int getDamage() const { return damage; }
    int getAreaSize() const { return areaSize; }
};

#endif