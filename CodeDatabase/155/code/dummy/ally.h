#include <vector>
#include "dummy.h"
#ifndef ALLYH
#define ALLYH

class map;

class ally: public dummy_creature, public AI
{
    int radius, agression;
    
    public:

        ally(int x = 0, int y = 0, int hp = 5, int dmg = 1, int rad = 3, int agr = 0);

        int got_hit(int);

        void change_agression(map*);

        std::vector<int> find_target(map*, int, int);

        void make_turn(map*);

        std::string save_parameters();

        std::string save_par_helper(int par, char sep);
        
        ~ally();
    
};

#endif