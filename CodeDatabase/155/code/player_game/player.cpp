#include <iostream>
#include <cmath>
#include "player.h"
#include "map.h"
#include "../spells/hand.h"

#define ERROR_LETT 'E'

player::player(int x, int y, int hp, int dmg, int exp, std::vector<int> al_pr): dummy_creature(x, y, hp, dmg)
{
    ally_props = al_pr;
    exp_points = exp;
    style = false;
    save = false;
    d_range_dmg = dmg - dmg / 2;
    skip = false;
    pl_hand = new hand();
}

player::player(hand* hand, int x, int y, int hp, int dmg, int exp, bool st, std::vector<int> al_pr): dummy_creature(x, y, hp, dmg)
{
    ally_props = al_pr;
    exp_points = exp;
    style = st;
    save = false;
    d_range_dmg = dmg - dmg / 2;
    skip = false;
    pl_hand = hand;
}

void player::set_dmg(int dmg)
{
    if (style)
    {
        change_dmg(d_range_dmg);
        style = false;
    }
    change_dmg(dmg - get_dmg());
}

int player::got_hit(int dmg)
{
    change_hp(-dmg);
    if (get_hp() <= 0)
    {
        std::cout << "GAME OVER!" << std::endl;
        return 1;
    }
    return 0;
}

void player::contact(std::string ch, int x, int y, map* map)
{
    if (ch == ENEMY || ch == ENEMY_BARRACS || ch == ENEMY_TOWER)
    {
        std::vector<enemy*> list_en = map->get_enemies();
        std::vector<enemy_barrack*> list_en_barr = map->get_enemy_barracks();
        std::vector<enemy_tower*> list_en_tower = map->get_enemy_towers();
        
        for (enemy* en : list_en)
        {
            if (en->get_pos_x() == x && en->get_pos_y() == y)
            {
                if (en->got_hit(get_dmg()))
                {
                    map->del_enemy(en);
                    map->replace_tile(PLAYER, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                    change_pos_x(x - get_pos_x());
                    change_pos_y(y - get_pos_y());
                    exp_points++;
                }
            }
        }
        for (enemy_barrack* en_barr : list_en_barr)
        {
            if (en_barr->get_pos_x() == x && en_barr->get_pos_y() == y)
            {
                if (en_barr->got_hit(get_dmg()))
                {
                    map->del_enemy_barrack(en_barr);
                    map->replace_tile(PLAYER, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                    change_pos_x(x - get_pos_x());
                    change_pos_y(y - get_pos_y());
                    exp_points += 3;
                }
            }
        }
        for (enemy_tower* en_tower : list_en_tower)
        {
            if (en_tower->get_pos_x() == x && en_tower->get_pos_y() == y)
            {
                if (en_tower->got_hit(get_dmg()))
                {
                    map->del_enemy_tower(en_tower);
                    map->replace_tile(PLAYER, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
                    change_pos_x(x - get_pos_x());
                    change_pos_y(y - get_pos_y());
                    exp_points += 3;
                }
            }
        }
    }
    else if (ch == SLOW)
    {
        map->replace_tile(PLAYER, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
        change_pos_x(x - get_pos_x());
        change_pos_y(y - get_pos_y());
        skip = true;
    }
    else if (ch == CHEST)
    {
        pl_hand->gen_spell();
        map->replace_tile(PLAYER, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
        change_pos_x(x - get_pos_x());
        change_pos_y(y - get_pos_y());
    }
}

bool player::range_attack(map* map, int dmg)
{
    std::string action; 
    int flag = 0;
    int count_en = 0, count_en_build = 0, count_en_tower = 0;
    int command;
    bool notdigit;
    std::vector<enemy*> enemies_to_shoot;
    std::vector<enemy_barrack*> en_barracks_to_shoot;
    std::vector<enemy_tower*> en_towers_to_shoot;

    for (enemy* tmp_en : map->get_enemies())
    {
        if (pow(tmp_en->get_pos_x() - get_pos_x(), 2) + pow(tmp_en->get_pos_y() - get_pos_y(), 2) <= pow(RANGEATTRAD, 2))
        {
            flag = 1;
            enemies_to_shoot.push_back(tmp_en);
            std::cout << count_en << ": enemy at " << tmp_en->get_pos_x() << " " << tmp_en->get_pos_y() << std::endl;
            count_en++;
        }
    }

    for (enemy_barrack* tmp_en_building : map->get_enemy_barracks())
    {
        if (pow(tmp_en_building->get_pos_x() - get_pos_x(), 2) + pow(tmp_en_building->get_pos_y() - get_pos_y(), 2) <= pow(RANGEATTRAD, 2))
        {
            flag = 1;
            en_barracks_to_shoot.push_back(tmp_en_building);
            std::cout << count_en + count_en_build << ": enemy barrak at " << tmp_en_building->get_pos_x() << " " << tmp_en_building->get_pos_y() << std::endl;
            count_en_build++;
        }
    }

    for (enemy_tower* tmp_en_tower : map->get_enemy_towers())
    {
        if (pow(tmp_en_tower->get_pos_x() - get_pos_x(), 2) + pow(tmp_en_tower->get_pos_y() - get_pos_y(), 2) <= pow(RANGEATTRAD, 2))
        {
            flag = 1;
            en_towers_to_shoot.push_back(tmp_en_tower);
            std::cout << count_en + count_en_build + count_en_tower << ": enemy tower at " << tmp_en_tower->get_pos_x() << " " << tmp_en_tower->get_pos_y() << std::endl;
            count_en_tower++;
        }
    }

    if (flag)
    {
        do
        {
            notdigit = false;
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
                std::cout << "Wrong input, type enemy number" << std::endl; 
                command = -1;
            }
            else
            {
                command = stoi(action.substr(0, 5));
                if ((command >= count_en + count_en_build + count_en_tower) || command < 0)
                {
                    std::cout << "Wrong input, type enemy number" << std::endl;
                }
            }
        } while ((command >= count_en + count_en_build + count_en_tower) || command < 0);
        if (command < count_en)
        {
            if (enemies_to_shoot[command]->got_hit(dmg))
            {
                map->del_enemy(enemies_to_shoot[command]);
            }
        }
        else if (command < count_en + count_en_build)
        {
            if (en_barracks_to_shoot[command - count_en]->got_hit(dmg))
            {
                map->del_enemy_barrack(en_barracks_to_shoot[command - count_en]);
            }
        }
        else if (command < count_en + count_en_build + count_en_tower)
        {
            if (en_towers_to_shoot[command - count_en - count_en_build]->got_hit(dmg))
            {
                map->del_enemy_tower(en_towers_to_shoot[command - count_en - count_en_build]);
            }
        }
    }
    else
    {
        std::cout << "No enemies around" << std::endl;
        return false;
    }
    return true;
}
//∘ ░ █ ♠ ▲ ♛ ☗ ♥ ♦
void player::make_turn(map* map)
{
    char action;
    bool notdigit = false;
    std::string actionstr; 
    int flag = 0, command = 0;
    std::string tmp_ch;
    int x = get_pos_x(), y = get_pos_y();
    if (!skip)
    {
        if (style)
        {
            std::cout << "You have switched to ranged combat\nWhat do you want to do? Shoot(0) or another move(1)?" << std::endl;
            std::cin >> actionstr;
            action = actionstr[0];
            if (actionstr.size() > 1)
            {
                action = ERROR_LETT;
            }
            actionstr = "";
            if (action == '0')
            {
                range_attack(map, get_dmg());
                return;
            }
        }
        do
        {
            std::cin >> actionstr;
            if (actionstr.size() > 1)
            {
                std::cout << "Wrong input, type one letter" << std::endl;
                action = ERROR_LETT;
            }
            else
            {
                action = actionstr[0];
                if (std::string(COMMAND_LIST).find(action) == std::string::npos)
                {
                    std::cout << "Wrong input, no such option" << std::endl;
                }
            }
        } while (std::string(COMMAND_LIST).find(action) == std::string::npos);
        switch (action)
        {
            case 'D':
            case 'd':
                x += 1;
                break;
            case 'A':
            case 'a':
                x -= 1;
                break;
            case 'W':  
            case 'w':
                y -= 1; 
                break;
            case 'S':
            case 's':
                y += 1;
                break;
            case 'K':
            case 'k':
                save = true;
                break;
            case 'C':
            case 'c':
                if (!style)
                {
                    change_dmg(-d_range_dmg);
                    style = true;
                }
                else
                {
                    change_dmg(d_range_dmg);
                    style = false;
                }
                break;
            case 'M':
            case 'm':
                if (pl_hand->get_spells_in_hand().size() > 0) 
                {
                    std::cout << "Choose a spell:" << std::endl;
                    pl_hand->print_hand();
                    do
                    {
                        notdigit = false;
                        std::cin >> actionstr;
                        for (char i : actionstr)
                        {
                            if (!isdigit(i))
                            {
                                notdigit = true;
                                break;
                            }
                        }
                        if (notdigit)
                        {
                            std::cout << "Wrong input, type spell number" << std::endl; 
                            command = -1;
                        }
                        else
                        {
                            command = stoi(actionstr.substr(0, 5));
                            if (command >= (pl_hand->get_spells_in_hand().size()) || (command < 0))
                            {
                                std::cout << "Wrong input, type spell number" << std::endl;
                            }
                        }
                    } while (command >= (pl_hand->get_spells_in_hand().size()) || (command < 0));
                    pl_hand->use_spell_in_hand(map, pl_hand->get_spells_in_hand()[command], command);
                }
                else
                {
                    std::cout << "You dont have spells" << std::endl;
                }
                break;
        }
        tmp_ch = map->get_tile(x, y).status;

        if (tmp_ch != EMPTY)
        {
            contact(tmp_ch, x, y, map);
        }
        else
        {
            map->replace_tile(PLAYER, get_pos_x(), get_pos_y(), x - get_pos_x(), y - get_pos_y(), this);
            change_pos_x(x - get_pos_x());
            change_pos_y(y - get_pos_y());
        }
    }
    else
    {
        skip = false;
    }
}

hand* player::get_hand()
{
    return pl_hand;
}

std::vector<int> player::get_ally_props()
{
    return ally_props;
}

void player::change_ally_hp(int hp)
{
    ally_props[0] = hp;
}

void player::change_ally_dmg(int dmg)
{
    ally_props[0] = dmg;
}

int player::get_exp()
{
    return exp_points;
}

void player::change_exp(int dexp)
{
    exp_points -= dexp;
}

void player::print_parametrs()
{
    std::cout << "HP: " << get_hp() << " DMG: " << get_dmg() << " EXP: " << exp_points << std::endl;
    std::cout << "Spells(M(m) to use):" << std::endl;
    pl_hand->print_hand();
}

bool player::is_save()
{
    if (save)
    {
        save = false;
        return true;
    }
    return false;
}

std::string player::save_parameters()
{
    std::string data;
    data += save_par_helper(get_pos_x(), DATA_SEP);
    data += save_par_helper(get_pos_y(), DATA_SEP);
    data += save_par_helper(get_hp(), DATA_SEP);
    data += save_par_helper(get_dmg(), DATA_SEP);
    data += save_par_helper(exp_points, DATA_SEP);
    data += save_par_helper(style, DATA_SEP);
    data += save_par_helper(ally_props[0], DATA_SEP);
    data += save_par_helper(ally_props[1], DATA_SEP2);
    data += pl_hand->save_hand();
    return data;
}

std::string player::save_par_helper(int par, char sep)
{
    return std::to_string(par) + sep;
}

player::~player()
{
    delete pl_hand;
}