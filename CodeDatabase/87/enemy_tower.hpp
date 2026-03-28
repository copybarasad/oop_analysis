#ifndef ENEMY_TOWER_HPP
#define ENEMY_TOWER_HPP
#include "position.hpp"
#include "player.hpp"
#include "gamefield.hpp"

class Enemy_tower{
    private:
        Position position;
        int damage;
        int attack_radius;
        int cooldown;
        int max_cooldown;
    public:
        Enemy_tower(Position pos, int dmag, int rad): position(pos), damage(dmag), attack_radius(rad), cooldown(0), max_cooldown(2) {}
        void update_cooldown();
        void attack_player_if_in_range(Player& player, Gamefield& field);
        Position get_position() const{return position;}
        int get_damage() const{return damage;}
        int get_attack_radius() const{return attack_radius;}

        int get_cooldown() const{return cooldown;}
        int get_max_cooldown() const{return max_cooldown;}
        
        void set_cooldown(int cd){cooldown = cd;}
        void set_max_cooldown(int mcd){max_cooldown = mcd;}
};

#endif