#include "dummy.h"
#ifndef ENEMYBARRACKH
#define ENEMYBARRACKH

class map;

class enemy;

class enemy_barrack: public dummy
{
    int health, steps_to_spawn, num_steps;
    
    public:

        enemy_barrack(int x = 0, int y = 0, int hp = 7, int steps_to_spawn = 5);

        int got_hit(int dmg);

        void step();

        int get_hp();

        void make_turn(map*);

        std::string save_parameters();

        std::string save_par_helper(int par, char sep);
    
        ~enemy_barrack();
    
};

#endif