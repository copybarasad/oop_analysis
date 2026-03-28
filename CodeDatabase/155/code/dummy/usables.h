#include "dummy.h"
#ifndef USABLESH
#define USABLESH

class trap: public dummy
{
    int damage;
    public:
        trap(int, int, int);

        void make_turn(map*);

        int got_hit(int);

        std::string save_parameters();

        std::string save_par_helper(int par, char sep);

        ~trap();
};

#endif