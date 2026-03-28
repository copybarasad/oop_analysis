#include <cstdlib>
#include <ctime> 
#include <iostream>
#include <random>
#include "enemy_tower.h"
#include "../spells/spells.h"
#include "../player_game/map.h"

enemy_tower::enemy_tower(int x, int y, int hp, int dmg, int rad, int steps) : dummy(x, y)
{
    health = hp;
    steps_to_shoot = steps;
    damage = dmg;
    att_range = rad;
    num_steps = steps;
}

void enemy_tower::step()
{
    if (steps_to_shoot < num_steps)
    {
        steps_to_shoot++;
    }
}

int enemy_tower::got_hit(int dmg)
{
    health -= dmg;
    if (health <= 0)
    {
        return 1;
    }
    return 0;
}

void enemy_tower::make_turn(map* map)
{
    int pl_x = map->get_pl_ptr()->get_pos_x();
    int pl_y = map->get_pl_ptr()->get_pos_y();

    damage_spell* dmg_spell = new damage_spell(2, 5);
    
    
    if (pow(pl_x - get_pos_x(), 2) + pow(pl_y - get_pos_y(), 2) <= pow(att_range, 2))
    {
        step();
        if (steps_to_shoot == num_steps)
        {
            dmg_spell->use_spell(map);
            steps_to_shoot = 0;
        }
    }
}

int enemy_tower::get_hp()
{
    return health;
}

int enemy_tower::get_dmg()
{
    return damage;
}

std::string enemy_tower::save_parameters()
{
    std::string data;
    data += save_par_helper(get_pos_x(), DATA_SEP);
    data += save_par_helper(get_pos_y(), DATA_SEP);
    data += save_par_helper(health, DATA_SEP);
    data += save_par_helper(damage, DATA_SEP2);
    return data;
}

std::string enemy_tower::save_par_helper(int par, char sep)
{
    return std::to_string(par) + sep;
}

enemy_tower::~enemy_tower(){}
