#pragma once
#include "spell.h"

class TrapSpell : public Spell{
    TrapSpell(unsigned int damage = 60){
        this->damage = damage;
        this->type = TRAP;
    }
    
};
