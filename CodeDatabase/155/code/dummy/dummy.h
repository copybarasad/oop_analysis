#include <vector>
#include "../player_game/saver.h"
#ifndef DUMMYH
#define DUMMYH

class map;

class Idummy
{
    public:
        virtual void make_turn(map*) = 0;

        virtual std::string save_parameters() = 0;

        virtual std::string save_par_helper(int, char) = 0;
};

class AI
{
    public:
        virtual void change_agression(map*) = 0;

        virtual std::vector<int> find_target(map*, int, int) = 0;
};

class dummy: public Idummy
{
    int pos_x, pos_y;
    
    public:
        dummy(int pos_x = 0, int pos_y = 0);

        int get_pos_x();

        int get_pos_y();

        void change_pos_x(int);

        void change_pos_y(int);

        virtual int got_hit(int) = 0;

        virtual void make_turn(map*) = 0;

        ~dummy();
};

class dummy_creature: public dummy
{   
    int health, damage;
    
    public:
        dummy_creature(int x = 0, int y = 0, int hp = 0, int dmg = 0);

        int get_hp();
        
        int get_dmg();

        void set_hp(int);

        void change_hp(int);
        
        void change_dmg(int);

        virtual int got_hit(int) = 0;

        virtual void make_turn(map*) = 0;

        ~dummy_creature();
};

#endif