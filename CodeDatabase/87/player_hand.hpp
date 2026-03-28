#ifndef PLAYER_HAND_HPP
#define PLAYER_HAND_HPP
#include "spell.hpp"
#include <vector>
#include <memory>
#include <random>

class Player_hand{
    private:
        std::vector<std::unique_ptr<Spell>> spells;
        int max_size;
        std::mt19937 gen;
    public:
        void add_random_spell();
        Player_hand(int hand_size);
        bool can_add_spell() const;
        void add_spell(std::unique_ptr<Spell> spell);
        std::unique_ptr<Spell> use_spell(int index);
        const std::vector<std::unique_ptr<Spell>>& get_spells() const{return spells;}
        int size() const{return spells.size();}
        int get_max_size() const{return max_size;}

        void clear();
};
#endif