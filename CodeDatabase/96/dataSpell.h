#pragma once
#include "SpellType.h"
//#include "../EmpowerSpell.h"



struct dataSpell{
    SpellType type;
    virtual ~dataSpell() = default;

};

struct empowerSpell : dataSpell{
    int bonus;

    
};

struct areaDamageSpell : dataSpell{
    int damage;
    int radius;
};

struct directDamageSpell : dataSpell{
    int damage;
    int range;
};

struct summAllySpell : dataSpell{
    int numAllies;
};

struct trapSpell : dataSpell{
    int damage;
    int range;
};