#include <vector>
#include "dummy.h"
#ifndef ENEMYH
#define ENEMYH

class map;

class trap;

class player;

class enemy: public dummy_creature, public AI
{
    int agression, agr_radius;
    
    public:

        enemy(int x = 0, int y = 0, int hp = 3, int dmg = 1, int agr = 0, int agr_rad = 3);

        int got_hit(int);

        void change_agression(map*);

        std::vector<int> find_target(map*, int, int);

        void make_turn(map*);

        std::string save_parameters();

        std::string save_par_helper(int, char);
        
        ~enemy();
    
};

#endif