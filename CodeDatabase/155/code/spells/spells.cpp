#include <iostream>
#include <string>
#include <cmath>
#include "spells.h"
#include "../player_game/map.h"
#include "hand.h"
#include "../dummy/usables.h"

//Damage
damage_spell::damage_spell(int dmg, int rad)
{
    damage = dmg; 
    radius = rad;
}

bool damage_spell::use_spell(map* map, hand* hand)
{
    if (map->get_pl_ptr()->range_attack(map, damage));
    {
        return true;
    }
    return false;
}

bool damage_spell::use_spell(map* map)
{
    map->get_pl_ptr()->got_hit(damage);
    return true;
}

void damage_spell::upgrade(int count)
{
    radius += count;
}

int damage_spell::get_level()
{
    return radius - DEF_DAM + 1;
}

std::string damage_spell::get_name()
{
    return DAM_SPELL;
}

damage_spell::~damage_spell(){}


//AOE
aoe_spell::aoe_spell(int dmg, int rad, int sqr)
{
    damage = dmg; 
    radius = rad;
    square = sqr;
}

bool aoe_spell::use_spell(map* map, hand* hand)
{
    int count = 0, x_pos = 0, y_pos = 0;
    int command;
    bool notdigit = false;
    std::string ch, action;
    dummy* en_ptr;
    enemy* tmp_en;
    enemy_barrack* tmp_en_barr;
    enemy_tower* tmp_en_tow;
    std::vector<std::vector<int>> pos; 
    std::cout << "Choose a tile:" << std::endl;
    for (int x = 0; x <= radius * 2; x++)
    {
        for (int y = 0; y <= radius * 2; y++)
        {
            x_pos = map->get_pl_ptr()->get_pos_x() + x - radius;
            y_pos = map->get_pl_ptr()->get_pos_y() + y - radius;
            if ((pow(x_pos - map->get_pl_ptr()->get_pos_x(), 2) + pow(y_pos - map->get_pl_ptr()->get_pos_y(), 2) <= pow(radius, 2)) && (((x_pos >= 0) && (x_pos < map->get_width())) && ((y_pos >= 0) && (y_pos < map->get_height()))))
            {
                pos.push_back({x_pos, y_pos});
                std::cout << count << ": " << x_pos << " " << y_pos << std::endl; 
                count++;
            }
        }
    }
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
            std::cout << "Wrong input, type tile number" << std::endl; 
            command = -1;
        }
        else
        {
            command = stoi(action.substr(0, 5));
            if ((command >= count) || (command < 0))
            {
                std::cout << "Wrong input, type tile number" << std::endl;
            }
        }
    } while ((command >= count) || (command < 0));
    for (int x = pos[command][0] - (square / 2); x <= pos[command][0] + (square / 2); x++)
    {
        for (int y = pos[command][1] - (square / 2); y <= pos[command][1] + (square / 2); y++)
        {
            ch = map->get_tile(x, y).status;
            en_ptr = map->get_tile(x, y).adress;
            if (ch == ENEMY)
            {
                tmp_en = dynamic_cast<enemy*>(en_ptr);
                if (tmp_en->got_hit(damage))
                {
                    map->del_enemy(tmp_en);
                }
            }
            else if (ch == ENEMY_BARRACS)
            {
                tmp_en_barr = dynamic_cast<enemy_barrack*>(en_ptr);
                if (tmp_en->got_hit(damage))
                {
                    map->del_enemy(tmp_en);
                }
            }
            else if (ch == ENEMY_TOWER)
            {
                tmp_en_tow = dynamic_cast<enemy_tower*>(en_ptr);
                if (tmp_en->got_hit(damage))
                {
                    map->del_enemy(tmp_en);
                }
            }
        }
    }
    return true;
}

void aoe_spell::upgrade(int count)
{
    square += count * 2;
}

int aoe_spell::get_level()
{
    return (square - DEF_AOE) / 2 + 1;
}

std::string aoe_spell::get_name()
{
    return AOE_SPELL;
}

aoe_spell::~aoe_spell(){}


//Trap
trap_spell::trap_spell(int dmg)
{
    damage = dmg;
}

bool trap_spell::use_spell(map* map, hand* hand)
{
    int count = 0, pos_x, pos_y;
    int command;
    bool notdigit = false;
    int pl_x = map->get_pl_ptr()->get_pos_x();
    int pl_y = map->get_pl_ptr()->get_pos_y();
    std::string action;
    std::vector<std::vector<int>> pos;
    std::cout << "Where do you want to place a trap?" << std::endl;
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            if ((abs(x) != abs(y)) && map->get_tile(x + pl_x, y + pl_y).status == EMPTY)
            {
                pos.push_back({x + pl_x, y + pl_y});
                std::cout << count << ": " << x + pl_x << " " << y + pl_y << std::endl;
                count++;
            }
        }
    }
    
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
            std::cout << "Wrong input, type tile number" << std::endl; 
            command = -1;
        }
        else
        {
            command = stoi(action.substr(0, 5));
            if ((command >= count) || (command < 0))
            {
                std::cout << "Wrong input, type tile number" << std::endl;
            }
        }
    } while ((command >= count) || (command < 0));
    pos_x = pos[command][0];
    pos_y = pos[command][1];
    trap* tmp_trap = new trap(pos_x, pos_y, damage);
    map->add_trap(tmp_trap, pos_x, pos_y);

    return true;
}

void trap_spell::upgrade(int count)
{
    damage += count;
}

int trap_spell::get_level()
{
    return damage - DEF_TRAP + 1;
}

std::string trap_spell::get_name()
{
    return TRAP_SPELL;
}

trap_spell::~trap_spell(){}


//Summon
summon_spell::summon_spell(int al_num)
{
    allies_number = al_num;
}

bool summon_spell::use_spell(map* map, hand* hand)
{
    int hp_ally = map->get_pl_ptr()->get_ally_props()[0];
    int dmg_ally = map->get_pl_ptr()->get_ally_props()[1];;
    int count = 0, pos_x, pos_y;
    int pl_x = map->get_pl_ptr()->get_pos_x();
    int pl_y = map->get_pl_ptr()->get_pos_y();
    int command;
    bool notdigit;
    std::string action;
    std::vector<std::vector<int>> pos;
    for (int i = 0; i < allies_number; i++)
    {
        count = 0;
        std::cout << "Where do you want to place an ally?" << std::endl;
        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                if ((abs(x) != abs(y)) && map->get_tile(x + pl_x, y + pl_y).status == EMPTY)
                {
                    pos.push_back({x + pl_x, y + pl_y});
                    std::cout << count << ": " << x + pl_x << " " << y + pl_y << std::endl;
                    count++;
                }
            }
        }
        
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
                std::cout << "Wrong input, type tile number" << std::endl; 
                command = -1;
            }
            else
            {
                command = stoi(action.substr(0, 5));
                if ((command >= count) || (command < 0))
                {
                    std::cout << "Wrong input, type tile number" << std::endl;
                }
            }
        } while ((command >= count) || (command < 0));
        
        pos_x = pos[command][0];
        pos_y = pos[command][1];
        ally* tmp_ally = new ally(pos_x, pos_y, hp_ally, dmg_ally);
        map->add_ally(tmp_ally, pos_x, pos_y);
    }
    return true;
}

void summon_spell::upgrade(int count)
{
    allies_number += count;
}

int summon_spell::get_level()
{
    return allies_number - DEF_SUMMON + 1;
}

std::string summon_spell::get_name()
{
    return SUMMON_SPELL;
}

summon_spell::~summon_spell(){}


//Upgrage
upgrade_spell::upgrade_spell(int upgr)
{
    upgrade_status = upgr;
}

bool upgrade_spell::use_spell(map* map, hand* hand)
{
    int command;
    bool notdigit;
    std::string action;
    if (hand->get_spells_in_hand().size() > 1)
    {
        std::cout << "Which spell do you want to upgrade?" << std::endl;
        for (int i = 0; i < hand->get_spells_in_hand().size(); i++)
        {
            if (this != hand->get_spells_in_hand()[i])
            {
                std::cout << i << ": " << hand->get_spells_in_hand()[i]->get_name() << " spell" << std::endl;
            }
        }
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
                std::cout << "Wrong input, type spell number" << std::endl; 
                command = -1;
            }
            else
            {
                command = stoi(action.substr(0, 5));
                if ((command >= (hand->get_spells_in_hand().size() - 1)) || (command < 0))
                {
                    std::cout << "Wrong input, type tile number" << std::endl;
                }
            }
        } while ((command >= (hand->get_spells_in_hand().size() - 1)) || (command < 0));
    }
    else
    {
        std::cout << "You dont have spells to upgrade" << std::endl;
        return false;
    }

    hand->get_spells_in_hand()[command]->upgrade(upgrade_status);
    return true;
}

void upgrade_spell::upgrade(int count)
{
    upgrade_status += count;
}

int upgrade_spell::get_level()
{
    return upgrade_status - DEF_UPGR + 1;
}

std::string upgrade_spell::get_name()
{
    return UPGRADE_SPELL;
}

upgrade_spell::~upgrade_spell(){}


