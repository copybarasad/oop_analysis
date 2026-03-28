#ifndef SPELL_AREA_HPP
#define SPELL_AREA_HPP
#include "spell.hpp"

class Area_spell: public Spell{
    public:
        Area_spell(std::string name, int dmag, int radi): Spell(std::move(name), dmag, radi){}
        bool is_valid_target(Position& player_pos, Position& target_pos) const override;
        void apply_effect(Player& player, Position& target_pos, std::vector<std::unique_ptr<Enemy>>& enemies, Gamefield& field) override;
};
#endif