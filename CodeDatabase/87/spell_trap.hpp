#ifndef SPELL_TRAP_HPP
#define SPELL_TRAP_HPP
#include "spell.hpp"

class Trap_spell: public Spell{
    public:
        Trap_spell(std::string name, int dmag, int radi): Spell(std::move(name), dmag, radi){}
        bool is_valid_target(Position& player_pos, Position& target_pos) const override;
        void apply_effect(Player& player, Position& target_pos, std::vector<std::unique_ptr<Enemy>>& enemies, Gamefield& field) override;
};
#endif