#include <random>
#include <iostream>
#include <string>
#include "ally.h"
#include "../player_game/map.h"

ally::ally(int x, int y, int hp, int dmg, int rad, int agr): dummy_creature(x, y, hp, dmg)
{
    radius = rad;
    agression = agr;
}

int ally::got_hit(int dmg)
{
    change_hp(-dmg);
    if (get_hp() <= 0)
    {
        return 1;
    }
    return 0;
}

void ally::change_agression(map* map)
{ 
    int x = get_pos_x();
    int y = get_pos_y();
    int en_x, en_y;
    int minr = pow(radius, 2) + 1;
    for (enemy* i : map->get_enemies())
    {
        en_x = i->get_pos_x();
        en_y = i->get_pos_y();
        int enr = pow((en_x - x), 2) + pow((en_y - y), 2);
        if (enr < minr)
        {
            minr = enr;
        }
    }
    if (minr <= pow(radius, 2))
    {
        agression = 1;
    }
    else
    {
        agression = 0;
    }
}

std::vector<int> ally::find_target(map* map, int x, int y)
{
    std::vector<std::vector<int>> dists;
    std::string tmp_ch;
    int en_x, en_y, target_x, target_y;
    int minr = pow(radius, 2);
    for (enemy* i : map->get_enemies())
    {
        en_x = i->get_pos_x();
        en_y = i->get_pos_y();
        int enr = pow((en_x - x), 2) + pow((en_y - y), 2);
        if (enr < minr)
        {
            minr = enr;
            target_x = en_x;
            target_y = en_y;
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
        if (tmp_ch == EMPTY || tmp_ch == ENEMY)
        {
            return i;
        }
    }
    return {0, 0};
}

void ally::make_turn(map* map)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> ally_diraction(0, 3);
    int random_num = ally_diraction(gen);

    dummy* tmp_adr;
    enemy* tmp_en;
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
                map->replace_tile(ALLY, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                change_pos_x(x - get_pos_x());
                change_pos_y(y - get_pos_y());
            }
            break;

        case 1:
            ddist = find_target(map, x, y);
            x += ddist[0];
            y += ddist[1];
            std::cout << ddist[0] << ddist[1] << std::endl;
            tmp_ch = map->get_tile(x, y).status;
            tmp_adr = map->get_tile(x, y).adress;
            if (tmp_ch == EMPTY)
            {
                map->replace_tile(ALLY, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                change_pos_x(x - get_pos_x());
                change_pos_y(y - get_pos_y());
            }
            else if (tmp_ch == ENEMY)
            {
                tmp_en = dynamic_cast<enemy*>(tmp_adr);
                if (tmp_en->got_hit(get_dmg()))
                {
                    map->del_enemy(tmp_en);
                    map->replace_tile(ALLY, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                    change_pos_x(x - get_pos_x());
                    change_pos_y(y - get_pos_y());
                }
            }
            break;
    }
}

std::string ally::save_parameters()
{
    std::string data;
    data += save_par_helper(get_pos_x(), DATA_SEP);
    data += save_par_helper(get_pos_y(), DATA_SEP);
    data += save_par_helper(get_hp(), DATA_SEP);
    data += save_par_helper(get_dmg(), DATA_SEP2);
    return data;
}

std::string ally::save_par_helper(int par, char sep)
{
    return std::to_string(par) + sep;
}

ally::~ally(){}