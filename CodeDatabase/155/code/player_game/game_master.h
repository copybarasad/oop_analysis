#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#ifndef GAMEMASTERH
#define GAMEMASTERH

#define MENU "0. New game\n1. Load game\n2. Info\n3. Exit"
#define HELP "Some help"
#define MENU_NUM 4
#define BONUS_HP_COST "5"
#define BONUS_DMG_COST "10"
#define BONUS_SPELL_COST "3"
#define EXP_GET "10"
#define BONUS_UPGRADE_COST "5"
#define BONUS_ALLY_HP_COST "7"
#define BONUS_ALLY_DMG_COST "4"

#define DEFUALT_EN_HP 3
#define DEFUALT_EN_DMG 1
#define DEFUALT_EN_BARR_HP 7
#define DEFUALT_EN_BARR_STEPS 5
#define DEFUALT_EN_TOW_HP 7
#define DEFUALT_EN_TOW_DMG 2
#define DEFUALT_EN_TOW_STEPS 5

#define DEFUALT_WALL_COUNT 10
#define DEFUALT_SLOW_COUNT 4
#define DEFUALT_CHEST_COUNT 2
#define DEFUALT_EN_COUNT 5
#define DEFUALT_EN_BARR_COUNT 1
#define DEFUALT_EN_TOW_COUNT 1

class map;

class player;

class saver;

struct Bonus
{
    std::string message;
    int id;
    int cost;
};

const Bonus bonus_hp = {"+5 health (cost: " BONUS_HP_COST " exp)", 80, std::stoi(BONUS_HP_COST)};
const Bonus bonus_dmg = {"+1 damage (cost: " BONUS_DMG_COST " exp)", 81, std::stoi(BONUS_DMG_COST)};
const Bonus bonus_spell = {"get random spell (cost: " BONUS_SPELL_COST " exp)", 82, std::stoi(BONUS_SPELL_COST)};
const Bonus bonus_exp = {"get " EXP_GET " exp for free", 83, 0};
const Bonus bonus_upgrade = {"upgrade spells (cost: " BONUS_UPGRADE_COST " exp)", 84, std::stoi(BONUS_UPGRADE_COST)};
const Bonus bonus_ally_hp = {"+2 health to your allies (cost: " BONUS_ALLY_HP_COST " exp)", 85, std::stoi(BONUS_ALLY_HP_COST)};
const Bonus bonus_ally_dmg = {"+1 damage to your allies (cost: " BONUS_ALLY_DMG_COST " exp)", 86, std::stoi(BONUS_ALLY_DMG_COST)};


class game_master
{
    std::vector<int> num_objs, en_par, en_barr_par, en_tow_par;
    int level;
    map* world;

    public:
        game_master();

        void menu();

        map* start_game();
        
        void main_cycle(map*);

        map* gen_new_level(player*, int);

        void endless_game(bool, int);

        void bonus();

        int get_level();

        std::string save_stats();

        std::string save_stats_helper(int, char);

        void set_all(map*, int, std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>);

        ~game_master();
};

#endif