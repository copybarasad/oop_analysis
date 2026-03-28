#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy: public Character{
    protected:
    unsigned int melee_damage;
    unsigned int points;
    
    public:
    Enemy(unsigned int game_lvl, unsigned int hard_lvl);
    
    Enemy(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp, bool move);

    Enemy(const Enemy& enemy);

    Enemy& operator= (const Enemy& enemy);

    unsigned int attack();

    unsigned int range();

    void change_move();

    unsigned int give_points();
};

#endif