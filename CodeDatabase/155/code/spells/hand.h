#include <vector>
#include <string>
#ifndef HANDH
#define HANDH

class Ispell;

class map;

class hand
{
    std::vector<Ispell*> spells_in_hand;

    int hand_size;

    public:
        hand(int hs = 5);

        hand(std::vector<std::string>, std::vector<int>);

        std::vector<Ispell*> get_spells_in_hand();

        void use_spell_in_hand(map*, Ispell*, int);

        void print_hand();

        void gen_spell();

        void add_spell(Ispell*);

        void del_spell(Ispell*, int);

        std::string save_hand();

        ~hand();
};

#endif