#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "saver.h"
#include "game_master.h"
#include "map.h"
#include "../spells/hand.h"

saver::saver(){}

void saver::save(const std::string& file_name, const std::string& content)
{
    std::ofstream file(file_name);
    if (!file.is_open())
    {
        throw std::runtime_error("Can`t open file " + file_name);
    }

    file << content;

    file.close();
}

std::string saver::load(const std::string& file_name)
{
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        throw std::runtime_error("Can`t open file " + file_name);
    }
    std::string data;
    std::string line;
    while (std::getline(file, line)) 
    {
        data += line + '\n';
    }

    file.close();
    return data;
}

std::string saver::data_cipher(map* map, game_master* gm)
{
    std::string data;
    player* pl = map->get_pl_ptr();
    //player
    data += pl->save_parameters();
    //level
    data += std::to_string(gm->get_level()) + DATA_SEP2;
    //stats
    data += gm->save_stats();
    //map
    data += map->save_parameters();
    return data;
}

void saver::data_decipher(std::string data, game_master* gm)
{
    if (std::string(data).find(DATA_SEP) == std::string::npos || std::string(data).find(DATA_SEP2) == std::string::npos)
    {
        throw std::runtime_error("Using broken file");
    }
    int pos_x, pos_y, hp, dmg, exp, w, h, level, style, num_spells;
    std::vector<std::string> names;
    std::vector<int> levels, num_objs, en_par, en_barr_par, en_tow_par;
    tile** tiles;
    std::vector<int> num;
    std::vector<int> al_pr = {0, 0};
    std::vector<ally*> allies;
    std::vector<enemy*> enemies;
    std::vector<enemy_barrack*> barracks;
    std::vector<enemy_tower*> towers;
    std::vector<trap*> traps;
    //player
    pos_x = data_decipher_helper(&data, DATA_SEP);
    pos_y = data_decipher_helper(&data, DATA_SEP);
    hp = data_decipher_helper(&data, DATA_SEP);
    dmg = data_decipher_helper(&data, DATA_SEP);
    exp = data_decipher_helper(&data, DATA_SEP);
    style = data_decipher_helper(&data, DATA_SEP);
    al_pr[0] = data_decipher_helper(&data, DATA_SEP);
    al_pr[1] = data_decipher_helper(&data, DATA_SEP2);
    num_spells = data_decipher_helper(&data, DATA_SEP2);
    for (int i = 0; i < num_spells; i++)
    {
        names.push_back(data_decipher_helper_str(&data, DATA_SEP));
        levels.push_back(data_decipher_helper(&data, DATA_SEP2));
    }
    hand* nhand = new hand(names, levels);
    player* pl = new player(nhand, pos_x, pos_y, hp, dmg, exp, style, al_pr);
    //level
    level = data_decipher_helper(&data, DATA_SEP2);
    //num_objs
    for (int i = 0; i < 6; i++)
    {
        num_objs.push_back(data_decipher_helper(&data, DATA_SEP));
    }
    data_decipher_helper_str(&data, DATA_SEP2);
    //en_par
    for (int i = 0; i < 2; i++)
    {
        en_par.push_back(data_decipher_helper(&data, DATA_SEP));
    }
    data_decipher_helper_str(&data, DATA_SEP2);
    //en_barr_par
    for (int i = 0; i < 1; i++)
    {
        en_barr_par.push_back(data_decipher_helper(&data, DATA_SEP));
    }
    data_decipher_helper_str(&data, DATA_SEP2);
    //en_tow_par
    for (int i = 0; i < 2; i++)
    {
        en_tow_par.push_back(data_decipher_helper(&data, DATA_SEP));
    }
    data_decipher_helper_str(&data, DATA_SEP2);
    //map
    w = data_decipher_helper(&data, DATA_SEP);
    h = data_decipher_helper(&data, DATA_SEP2);
    //tiles
    tiles = new tile*[h];
    for (int y = 0; y < h; y++)
    {
        tiles[y] = new tile[w];
    }
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            tiles[y][x].status = data_decipher_helper_str(&data, DATA_SEP);
            tiles[y][x].adress = nullptr;
        }
        data_decipher_helper_str(&data, DATA_SEP2);
    }
    //num_ent
    for (int i = 0; i < 5; i++)
    {
        num.push_back(data_decipher_helper(&data, DATA_SEP));
    }
    data_decipher_helper_str(&data, DATA_SEP2);
    //enemies
    for (int i = 0; i < num[0]; i++)
    {
        pos_x = data_decipher_helper(&data, DATA_SEP);
        pos_y = data_decipher_helper(&data, DATA_SEP);
        hp = data_decipher_helper(&data, DATA_SEP);
        dmg = data_decipher_helper(&data, DATA_SEP2);
        enemy* en = new enemy(pos_x, pos_y, hp, dmg);
        tiles[pos_y][pos_x].adress = en;
        enemies.push_back(en);
    }
    //enemy_barracks
    for (int i = 0; i < num[1]; i++)
    {
        pos_x = data_decipher_helper(&data, DATA_SEP);
        pos_y = data_decipher_helper(&data, DATA_SEP);
        hp = data_decipher_helper(&data, DATA_SEP2);
        enemy_barrack* en_barr = new enemy_barrack(pos_x, pos_y, hp);
        tiles[pos_y][pos_x].adress = en_barr;
        barracks.push_back(en_barr);
    }
    //enemy_towers
    for (int i = 0; i < num[2]; i++)
    {
        pos_x = data_decipher_helper(&data, DATA_SEP);
        pos_y = data_decipher_helper(&data, DATA_SEP);
        hp = data_decipher_helper(&data, DATA_SEP);
        dmg = data_decipher_helper(&data, DATA_SEP2);
        enemy_tower* en_tow = new enemy_tower(pos_x, pos_y, hp, dmg);
        tiles[pos_y][pos_x].adress = en_tow;
        towers.push_back(en_tow);
    }
    //allies
    for (int i = 0; i < num[3]; i++)
    {
        pos_x = data_decipher_helper(&data, DATA_SEP);
        pos_y = data_decipher_helper(&data, DATA_SEP);
        hp = data_decipher_helper(&data, DATA_SEP);
        dmg = data_decipher_helper(&data, DATA_SEP2);
        ally* al = new ally(pos_x, pos_y, hp, dmg);
        tiles[pos_y][pos_x].adress = al;
        allies.push_back(al);
    }
    //traps
    for (int i = 0; i < num[4]; i++)
    {
        pos_x = data_decipher_helper(&data, DATA_SEP);
        pos_y = data_decipher_helper(&data, DATA_SEP);
        dmg = data_decipher_helper(&data, DATA_SEP2);
        trap* tr = new trap(pos_x, pos_y, dmg);
        tiles[pos_y][pos_x].adress = tr;
        traps.push_back(tr);
    }
    map* nmap = new map(pl, w, h, tiles, allies, enemies, barracks, towers, traps);
    gm->set_all(nmap, level, num_objs, en_par, en_barr_par, en_tow_par);
}

int saver::data_decipher_helper(std::string* data, char data_sep)
{
    int pos;
    std::string sub;
    pos = data->find(data_sep);
    sub = data->substr(0, pos);
    data->erase(0, sub.size() + 1);
    try
    {
        stoi(sub);
    }
    catch(...)
    {
        throw std::runtime_error("Using broken file");
    }
    return stoi(sub);
}

std::string saver::data_decipher_helper_str(std::string* data, char data_sep)
{
    int pos;
    std::string sub;
    pos = data->find(data_sep);
    sub = data->substr(0, pos);
    data->erase(0, sub.size() + 1);
    if ((std::string(TILES_LIST).find(sub) == std::string::npos) && data->size() == 1)
    {
        throw std::runtime_error("Using broken file");
    }
    return sub;
}

saver::~saver(){}
