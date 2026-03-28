#ifndef SPELL_HPP
#define SPELL_HPP
#include "position.hpp"
#include <string>
#include <vector>
#include <memory>

class Player;
class Enemy;
class Gamefield;
class Spell{
    protected:
        std::string name;
        int damage;
        int radius;
    public:
        Spell(std::string spell_name, int dmag, int radi): name(std::move(spell_name)), damage(dmag), radius(radi) {}
        virtual ~Spell() = default;
        virtual bool is_valid_target(Position& player_pos, Position& target_pos) const = 0;
        virtual void apply_effect(Player& player, Position& target_pos, std::vector<std::unique_ptr<Enemy>>& enemies, Gamefield& field) = 0;
        std::string get_name() const{return name;}
        int get_damage() const{return damage;}
        int get_radius() const{return radius;}
        virtual void add_damage(int amount){damage += amount;}
};
#endif