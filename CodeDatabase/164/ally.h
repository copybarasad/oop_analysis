#ifndef ALLY_H
#define ALLY_H

#include "character.h"

class Ally: public Character{
    protected:
    unsigned int melee_damage;
    
    public:
    Ally(unsigned int game_lvl, unsigned int hard_lvl);

    Ally(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp, bool move);

    Ally(const Ally& ally): Character(ally), melee_damage(ally.melee_damage);

    Ally& operator= (const  Ally& ally);

    unsigned int attack();

    unsigned int range();

    void change_move();
};

#endif