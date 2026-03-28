//
// Created by Artem on 30.10.2025.
//

#ifndef LABAOOP2_SPELLONCOORDS_H
#define LABAOOP2_SPELLONCOORDS_H

#include "Spell.h"
#include "../../../../Constants/Constants.h"
class SpellOnCoords : public Spell {
public:
    explicit SpellOnCoords(SpellType type) : Spell(type) {}
    virtual void castSpell(int gradeLevel, Field& field, Constants::XYPair from, Constants::XYPair to) const = 0;
};


#endif //LABAOOP2_SPELLONCOORDS_H
