#include <cstdlib>
#include <ctime> 
#include <iostream>
#include <random>
#include "enemy_barrack.h"
#include "enemy.h"
#include "../player_game/map.h"

enemy_barrack::enemy_barrack(int x, int y, int hp, int steps) : dummy(x, y)
{
    health = hp;
    steps_to_spawn = steps;
    num_steps = steps;
}

void enemy_barrack::step()
{
    if (steps_to_spawn > 0)
    {
        steps_to_spawn--;
    }
}

int enemy_barrack::got_hit(int dmg)
{
    health -= dmg;
    if (health <= 0)
    {
        return 1;
    }
    return 0;
}

void enemy_barrack::make_turn(map* map)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> enem_pos_x(0, 2);
    static std::uniform_int_distribution<int> enem_pos_y(0, 2);

    int spos_x = enem_pos_x(gen) - 1 + get_pos_x();
    int spos_y = enem_pos_y(gen) - 1 + get_pos_y();
    step();
    if (steps_to_spawn == 0)
    {
        if (map->get_tile(spos_x, spos_y).status == EMPTY)
        {
            enemy* tmp_adress = new enemy(spos_x, spos_y, 3, 1, 0, 3);
            map->add_enemy(tmp_adress, spos_x, spos_y);
            steps_to_spawn = num_steps;
            std::cout << "spawned enemy at: " << spos_x << " " << spos_y << std::endl;
        }
    }
}

int enemy_barrack::get_hp()
{
    return health;
}

std::string enemy_barrack::save_parameters()
{
    std::string data;
    data += save_par_helper(get_pos_x(), DATA_SEP);
    data += save_par_helper(get_pos_y(), DATA_SEP);
    data += save_par_helper(health, DATA_SEP2);
    return data;
}

std::string enemy_barrack::save_par_helper(int par, char sep)
{
    return std::to_string(par) + sep;
}

enemy_barrack::~enemy_barrack(){}
