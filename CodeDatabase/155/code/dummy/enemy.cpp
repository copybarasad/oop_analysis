#include <random>
#include <iostream>
#include <string>
#include "enemy.h"
#include "../player_game/map.h"

enemy::enemy(int x, int y, int hp, int dmg, int agr, int agr_rad): dummy_creature(x, y, hp, dmg)
{
    agression = agr;
    agr_radius = agr_rad;
}

int enemy::got_hit(int dmg)
{
    change_hp(-dmg);
    if (get_hp() <= 0)
    {
        return 1;
    }
    return 0;
}

void enemy::change_agression(map* map)
{ 
    int x = get_pos_x();
    int y = get_pos_y();
    int ally_x, ally_y;
    int minr = pow((map->get_pl_ptr()->get_pos_x() - x), 2) + pow((map->get_pl_ptr()->get_pos_y() - y), 2);
    for (ally* i : map->get_allies())
    {
        ally_x = i->get_pos_x();
        ally_y = i->get_pos_y();
        int allyr = pow((ally_x - x), 2) + pow((ally_y - y), 2);
        if (allyr < minr)
        {
            minr = allyr;
        }
    }
    if (minr <= pow(agr_radius, 2))
    {
        agression = 1;
    }
    else
    {
        agression = 0;
    }
}

std::vector<int> enemy::find_target(map* map, int x, int y)
{
    std::vector<std::vector<int>> dists;
    std::string tmp_ch;
    int target_x = map->get_pl_ptr()->get_pos_x();
    int target_y = map->get_pl_ptr()->get_pos_y();
    int ally_x, ally_y;
    int minr = pow((target_x - x), 2) + pow((target_y - y), 2);
    for (ally* i : map->get_allies())
    {
        ally_x = i->get_pos_x();
        ally_y = i->get_pos_y();
        int allyr = pow((ally_x - x), 2) + pow((ally_y - y), 2);
        if (allyr < minr)
        {
            minr = allyr;
            target_x = ally_x;
            target_y = ally_y;
        }
    }

    for (int dx : {1, 0, -1})
    {
        for (int dy : {1, 0, -1})
        {
            if (abs(dx) != abs(dy))
            {
                int cur_r = pow((target_x - (x + dx)), 2) + pow((target_y - (y + dy)), 2);
                if (cur_r < minr)
                {
                    minr = cur_r;
                    dists.push_back({dx, dy});
                }
            }
        }
    }

    for (std::vector<int> i : dists)
    {
        tmp_ch = map->get_tile(x + i[0], y + i[1]).status;
        if (tmp_ch == EMPTY || tmp_ch == TRAP || tmp_ch == PLAYER || tmp_ch == ALLY)
        {
            return i;
        }
    }
    return {0, 0};
}

void enemy::make_turn(map* map)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> en_diraction(0, 3);
    int random_num = en_diraction(gen);

    player* pl = map->get_pl_ptr();
    bool del_flag = false;
    dummy* tmp_adr;
    ally* tmp_ally;
    std::string tmp_ch;
    std::vector<int> ddist;
    int x = get_pos_x();
    int y = get_pos_y();
    change_agression(map);
    switch(agression)
    {
        case 0:
            switch(random_num)
            {
                case 0:
                    x += 1;
                    break;
                case 1:
                    x -= 1;
                    break;
                case 2:
                    y += 1;
                    break;
                case 3:
                    y -= 1;
                    break;
            }
            tmp_ch = map->get_tile(x, y).status;
            tmp_adr = map->get_tile(x, y).adress;
            if (tmp_ch == EMPTY)
            {
                map->replace_tile(ENEMY, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                change_pos_x(x - get_pos_x());
                change_pos_y(y - get_pos_y());
            }
            else if (tmp_ch == TRAP)
            {
                if (this->got_hit(tmp_adr->got_hit(0)))
                {
                    del_flag = true;
                }
                map->del_trap(dynamic_cast<trap*>(tmp_adr));
                map->replace_tile(ENEMY, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                change_pos_x(x - get_pos_x());
                change_pos_y(y - get_pos_y());
                if (del_flag)
                {
                    map->del_enemy(this);
                }
            }
            break;

        case 1:
            ddist = find_target(map, x, y);
            x += ddist[0];
            y += ddist[1];
            tmp_ch = map->get_tile(x, y).status;
            tmp_adr = map->get_tile(x, y).adress;
            if (tmp_ch == EMPTY)
            {
                map->replace_tile(ENEMY, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                change_pos_x(x - get_pos_x());
                change_pos_y(y - get_pos_y());
            }
            else if (tmp_ch == TRAP)
            {
                if (this->got_hit(tmp_adr->got_hit(0)))
                {
                    del_flag = true;
                }
                map->del_trap(dynamic_cast<trap*>(tmp_adr));
                map->replace_tile(ENEMY, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                change_pos_x(x - get_pos_x());
                change_pos_y(y - get_pos_y());
                if (del_flag)
                {
                    map->del_enemy(this);
                }
            }
            else if (tmp_ch == PLAYER)
            {
                pl->got_hit(get_dmg());
            }
            else if (tmp_ch == ALLY)
            {
                tmp_ally = dynamic_cast<ally*>(tmp_adr);
                if (tmp_ally->got_hit(get_dmg()))
                {
                    map->del_ally(tmp_ally);
                    map->replace_tile(ENEMY, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                    change_pos_x(x - get_pos_x());
                    change_pos_y(y - get_pos_y());
                }
            }
            break;
    }
}

std::string enemy::save_parameters()
{
    std::string data;
    data += save_par_helper(get_pos_x(), DATA_SEP);
    data += save_par_helper(get_pos_y(), DATA_SEP);
    data += save_par_helper(get_hp(), DATA_SEP);
    data += save_par_helper(get_dmg(), DATA_SEP2);
    return data;
}

std::string enemy::save_par_helper(int par, char sep)
{
    return std::to_string(par) + sep;
}

enemy::~enemy(){}
    