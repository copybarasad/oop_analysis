#ifndef HERO_H
#define HERO_H

#include "character.h"

enum class attack_type{
    melee,
    ranged
};

class Hero: public Character{
    protected:
    unsigned int melee_damage;
    unsigned int ranged_damage;
    unsigned int attack_range;
    unsigned int money;
    attack_type curent_attack;
    
    public:
    Hero(unsigned int game_lvl, unsigned int hard_lvl);

    Hero(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp, unsigned int money, bool move);

    Hero(const Hero& hero);

    Hero& operator= (const Hero& hero);

    void change_attack(attack_type attack);

    unsigned int attack();

    unsigned int range();

    void get_money(unsigned int points);
    
    void change_move();
    
    bool win(unsigned int win_points);
};

#endif