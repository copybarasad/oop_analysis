#ifndef TOWER_H
#define TOWER_H

#include "character.h"

class Tower: public Character{
    protected:
    unsigned int ranged_damage;
    unsigned int attack_range;
    unsigned int period_action;
    unsigned int period_counter;
    unsigned int points;
    
    public:
    Tower(unsigned int game_lvl, unsigned int hard_lvl);

    Tower(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp);

    Tower(const Tower& tower);

    Tower& operator= (const Tower& tower);

    unsigned int attack();

    unsigned int range();

    unsigned int give_points();
};

#endif