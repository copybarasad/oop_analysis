#include <string>
#include <random>
#include <algorithm>
#include <filesystem>
#include "../spells/hand.h"
#include "../spells/spells.h"
#include "command_proc.h"
#include "render.h"
#include "game_master.h"
#include "saver.h"
#include "map.h"


template<typename input>
class game_controller;

game_master::game_master()
{
    num_objs = {DEFUALT_WALL_COUNT, DEFUALT_SLOW_COUNT, DEFUALT_CHEST_COUNT, DEFUALT_EN_COUNT, DEFUALT_EN_BARR_COUNT, DEFUALT_EN_TOW_COUNT};
    en_par = {DEFUALT_EN_HP, DEFUALT_EN_DMG};
    en_barr_par = {DEFUALT_EN_BARR_HP};
    en_tow_par = {DEFUALT_EN_TOW_HP, DEFUALT_EN_TOW_DMG};
    world = nullptr;
    level = 0;
}

void game_master::menu()
{
    std::cout << MENU << std::endl;
    game_controller<command_proc> control(this);
    control.process_input();
}

map* game_master::start_game()
{
    player* pl = new player(0, 0, 20);
    world = gen_new_level(pl, 0);
    return world;
}

void game_master::endless_game(bool load, int level)
{   
    game_renderer<console_renderer>* rend = new game_renderer<console_renderer>();  
    if (!load)
    {
        world = start_game();
    }
    rend->renderer(world);
    player* pl = world->get_pl_ptr();
    pl->print_parametrs();
    while (true)
    {
        main_cycle(world);
        level++;
        world = gen_new_level(pl, level);
        rend->renderer(world);
        pl->print_parametrs();
    }
}

map* game_master::gen_new_level(player* pl, int lvl)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<double> width(16, 2.5);
    static std::normal_distribution<double> height(14, 2.5);
    int w = width(gen);
    int h = height(gen);
    std::normal_distribution<double> rand_pl_pos_x(w / 2, 3);
    std::normal_distribution<double> rand_pl_pos_y(h / 2, 3);
    int pl_pos_x = rand_pl_pos_x(gen);
    int pl_pos_y = rand_pl_pos_y(gen);
    std::normal_distribution<double> param_viriety_1(50, 20 + lvl);
    std::normal_distribution<double> param_viriety_2(50, 20 + lvl);
    std::normal_distribution<double> en_viriety(50, 20 + lvl);
    std::uniform_int_distribution<int> en_barr_viriety(0, 10 + lvl);
    std::normal_distribution<double> en_tow_viriety(50, 20 + lvl);
    
    if (lvl > 0)
    {
        if (param_viriety_1(gen) > 80)
        {
            if (num_objs[4] < 3)
            {
                std::cout << "More barracks" << std::endl;
                num_objs[4] += 1;
            }
        }
        else if (param_viriety_1(gen) < 20)
        {
            if (num_objs[5] < 5)
            {
                std::cout << "More towers" << std::endl;
                num_objs[5] += 1;
            }
        }

        if (param_viriety_2(gen) < 25)
        {
            if (num_objs[1] < 10)
            {
                std::cout << "More slows" << std::endl;
                num_objs[1] += 1;
            }
        }
        else if (param_viriety_2(gen) > 75)
        {
            if (num_objs[3] < 15)
            {
                std::cout << "More enemies" << std::endl;
                num_objs[3] += 1;
            }
        }
        else if (param_viriety_2(gen) > 80)
        {
            if (num_objs[2] < 5)
            {
                std::cout << "More chests" << std::endl;
                num_objs[2] += 1;
            }
        }
        int en = en_viriety(gen);
        int en_barr = en_barr_viriety(gen);
        int en_tow = en_tow_viriety(gen);
        if (en > 75)
        {
            std::cout << "More enemy health" << std::endl;
            en_par[0] += 1;
        }
        if (en < 25)
        {
            std::cout << "More enemy damage" << std::endl;
            en_par[1] += 1;
        }

        if (en_barr == 0)
        {
            std::cout << "More enemy barrack health" << std::endl;
            en_par[0] += 1;
        }

        if (en_tow > 75)
        {
            std::cout << "More enemy tower health" << std::endl;
            en_par[0] += 1;
        }
        if (en_tow < 25)
        {
            std::cout << "More enemy tower damage" << std::endl;
            en_par[1] += 1;
        }
    }

    if (w < 10)
    {
        w = 10;
    }
    else if (w > 25)
    {
        w = 25;
    }
    if (h < 10)
    {
        h = 10;
    }
    else if (h > 25)
    {
        h = 25;
    }
    

    if (pl_pos_x < 0)
    {
        pl_pos_x = 0;
    }
    else if (pl_pos_x > w)
    {
        pl_pos_x = w;
    }
    if (pl_pos_y < 0)
    {
        pl_pos_y = 0;
    }
    else if (pl_pos_y > h)
    {
        pl_pos_y = h;
    }

    map* new_world = new map(w, h, pl);
    pl->change_pos_x(-pl->get_pos_x() + pl_pos_x);
    pl->change_pos_y(-pl->get_pos_y() + pl_pos_y);
    new_world->generate(num_objs, en_par, en_barr_par, en_tow_par, pl);
    return new_world;
}

void game_master::main_cycle(map* map)
{
    game_renderer<console_renderer>* rend = new game_renderer<console_renderer>(); 
    player* pl = map->get_pl_ptr();
    int save_pl_hp = pl->get_hp();
    int save_pl_dmg = pl->get_dmg();
    int save_ally_hp = pl->get_ally_props()[0];
    int save_ally_dmg = pl->get_ally_props()[1];
    while (map->get_enemy_barracks().size() > 0 || map->get_enemy_towers().size() > 0)
    {
        pl->make_turn(map);
        if (pl->is_save())
        {
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            int time = static_cast<uint64_t>(milliseconds);
            saver* save = new saver();
            std::string data = save->data_cipher(map, this);
            save->save("D:/code/OOP/player_game/loadfiles/data_" + std::to_string(-time) + ".txt", data);
            exit(0);
        }
        for (ally* al : map->get_allies())
        {
            al->make_turn(map);
        }
        for (enemy* en : map->get_enemies())
        {
            en->make_turn(map);
        }
        for (enemy_barrack* barr : map->get_enemy_barracks())
        {
            barr->make_turn(map);
        }
        for (enemy_tower* tower : map->get_enemy_towers())
        {
            tower->make_turn(map);
        }
        rend->renderer(world);
        pl->print_parametrs();
    }
    std::cout << "\n\n☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆\nLevel complited!\n☆ ☆ ☆ ☆ ☆ ☆ ☆ ☆\n\nBut you also lost half of your spells" << std::endl;
    hand* pl_hand = pl->get_hand();
    std::vector<int> del_spells;
    std::vector<Bonus> bonuses;
    std::string action;
    int command, cost;
    bool notdigit = false;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> rspell(0,  pl_hand->get_spells_in_hand().size() - 1);
    std::uniform_int_distribution<int> bonus(0, 6);
    for (int i = 0; i < pl_hand->get_spells_in_hand().size() / 2; i++)
    {
        int ind = rspell(gen);
        auto it = std::find(del_spells.begin(), del_spells.end(), ind);
        if (it == del_spells.end())
        {
            del_spells.push_back(ind);
        }
        else
        {
            i--;
        }
    }
    std::sort(del_spells.begin(), del_spells.end(), std::greater<int>());
    for (int i = 0; i < del_spells.size(); i++)
    {
        pl_hand->del_spell(pl_hand->get_spells_in_hand()[del_spells[i]], del_spells[i]);
    }
    pl->print_parametrs();
    std::cout << "\nChoose a bonus:" << std::endl;
    for (int i = 0; i < 3; i++)
    {
        int curbonus = bonus(gen);
        switch (curbonus)
        {
            case 0:
                std::cout << i << ": " << bonus_hp.message << std::endl;
                bonuses.push_back(bonus_hp);
                break;
            case 1:
                std::cout << i << ": " << bonus_dmg.message << std::endl;
                bonuses.push_back(bonus_dmg);
                break;
            case 2:
                std::cout << i << ": " << bonus_upgrade.message << std::endl;
                bonuses.push_back(bonus_upgrade);
                break;
            case 3:
                std::cout << i << ": " << bonus_spell.message << std::endl;
                bonuses.push_back(bonus_spell);
                break;
            case 4:
                std::cout << i << ": " << bonus_exp.message << std::endl;
                bonuses.push_back(bonus_exp);
                break;
            case 5:
                std::cout << i << ": " << bonus_ally_hp.message << std::endl;
                bonuses.push_back(bonus_ally_hp);
                break;
            case 6:
                std::cout << i << ": " << bonus_ally_dmg.message << std::endl;
                bonuses.push_back(bonus_ally_dmg);
                break;
        }
    }
    do
    {
        std::cin >> action;
        for (char i : action)
        {
            if (!isdigit(i))
            {
                notdigit = true;
                break;
            }
        }
        if (notdigit)
        {
            std::cout << "Wrong input, type bonus number" << std::endl; 
            command = -1;
        }
        else
        {
            command = stoi(action.substr(0, 5));
            cost = bonuses[command].cost;
            if ((command >= (bonuses.size())) || (command < 0))
            {
                std::cout << "Wrong input" << std::endl;
            }
            if (pl->get_exp() < cost)
            {
                std::cout << "You don`t have enough exp points" << std::endl;
                std::cout << std::endl;
            }
        }
    } while ((command >= (bonuses.size())) || (command < 0) || (pl->get_exp() < cost));
    pl->change_exp(bonuses[command].cost);
    pl->set_hp(save_pl_hp);
    pl->set_dmg(save_pl_dmg);
    switch (bonuses[command].id)
    {
        case 80:
            pl->set_hp(save_pl_hp + 5);
            break;
        case 81:
            pl->set_dmg(save_pl_dmg + 1);
            break;
        case 82:
            pl_hand->gen_spell();
            break;
        case 83:
            pl->change_exp(-std::stoi(EXP_GET));
            break;
        case 84:
            for (Ispell* i : pl_hand->get_spells_in_hand())
            {
                i->upgrade(1);
            }
            break;
        case 85:
            pl->change_ally_hp(pl->get_ally_props()[0] + 2);
            break;
        case 86:
            pl->change_ally_dmg(pl->get_ally_props()[1] + 1);
            break;
    }
    pl->print_parametrs();
}

int game_master::get_level()
{
    return level;
}

std::string game_master::save_stats()
{
    std::string stats;
    for (int i : num_objs)
    {
        stats += save_stats_helper(i, DATA_SEP);
    }
    stats += DATA_SEP2;
    for (int i : en_par)
    {
        stats += save_stats_helper(i, DATA_SEP);
    }
    stats += DATA_SEP2;
    for (int i : en_barr_par)
    {
        stats += save_stats_helper(i, DATA_SEP);
    }
    stats += DATA_SEP2;
    for (int i : en_tow_par)
    {
        stats += save_stats_helper(i, DATA_SEP);
    }
    stats += DATA_SEP2;
    return stats;
}

std::string game_master::save_stats_helper(int par, char sep)
{
    return std::to_string(par) + sep;
}

void game_master::set_all(map* map, int lvl, std::vector<int> num_o, std::vector<int> en_p, std::vector<int> en_barr_p, std::vector<int> en_tow_p)
{
    world = map;
    level = lvl;
    num_objs = num_o;
    en_par = en_p;
    en_barr_par = en_barr_p;
    en_tow_par = en_tow_p;
}

game_master::~game_master(){}
