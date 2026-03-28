#include <string>
#include "../dummy/dummy.h"
#ifndef PLAYERH
#define PLAYERH

#define RANGEATTRAD 5
#define COMMAND_LIST "AaWwSsDdCcMmKk"


class map;

class hand;

class command_proc;

class player: public dummy_creature
{
    int exp_points, d_range_dmg;
    std::vector<int> ally_props;
    bool skip, style, save;
    hand* pl_hand;
    
    public:
        player(int x = 0, int y = 0, int hp = 10, int dmg = 2, int exp = 0, std::vector<int> al_pr = {5, 1});

        player(hand*, int, int, int, int, int, bool, std::vector<int>);

        bool range_attack(map*, int);

        void make_turn(map*);

        void print_parametrs();

        int got_hit(int);

        int get_exp();

        void change_exp(int);

        void set_dmg(int);

        hand* get_hand();

        void contact(std::string, int, int, map*);

        std::vector<int> get_ally_props();

        void change_ally_hp(int);

        void change_ally_dmg(int);

        std::string save_parameters();

        std::string save_par_helper(int, char);

        bool is_save();

        ~player();
};

#endif