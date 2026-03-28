#ifndef BASE_H
#define BASE_H

#include "character.h"

class Base: public Character{
    protected:
    unsigned int period_action;
    unsigned int period_counter;
    unsigned int points;
    
    public:
    Base(unsigned int game_lvl, unsigned int hard_lvl);
    
    Base(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp);

    Base(const Base& base);

    Base& operator= (const Base& base);

    bool make_enemy();

    unsigned int give_points();
};

#endif