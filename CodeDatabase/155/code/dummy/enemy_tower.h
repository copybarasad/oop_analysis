#include "dummy.h"
#ifndef ENEMYTOWERH
#define ENEMYTOWERH

class map;

class enemy_tower: public dummy
{
    int health, damage, att_range, steps_to_shoot, num_steps;
    
    public:

        enemy_tower(int x = 0, int y = 0, int hp = 7, int dmg = 2, int rad = 3, int steps_to_spawn = 5);

        int got_hit(int dmg);

        void step();

        int get_hp();

        int get_dmg();

        void make_turn(map*);

        std::string save_parameters();

        std::string save_par_helper(int par, char sep);
        
        ~enemy_tower();
    
};

#endif