#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include "map.h"
#include "player.h"
#include "../dummy/enemy.h"
#include "../dummy/enemy_barrack.h"
#include "../dummy/enemy_tower.h"



map::map(int width, int height, player* pl)
    :_width(width), _height(height), player_ptr(pl)
{
    _tiles = new tile*[_height];
    for (int y = 0; y < _height; y++)
    {
        _tiles[y] = new tile[_width];
    }
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            _tiles[y][x].status = EMPTY;
            _tiles[y][x].adress = nullptr;
        }
    }
}

map::map(player* pl, int width, int height, tile** tiles, std::vector<ally*> al, std::vector<enemy*> en, std::vector<enemy_barrack*> en_bar, std::vector<enemy_tower*> en_tow, std::vector<trap*> tr)
    :_width(width), _height(height), player_ptr(pl)
{
    _tiles = new tile*[_height];
    for (int y = 0; y < _height; y++)
    {
        _tiles[y] = new tile[_width];
    }
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            _tiles[y][x].status = tiles[y][x].status;
            _tiles[y][x].adress = tiles[y][x].adress;
        }
    }
    allies = al;
    enemies = en;
    barracks = en_bar;
    towers = en_tow;
    traps = tr;
}

int map::get_height()
{
    return _height;
}

int map::get_width()
{
    return _width;
}

tile map::get_tile(int x, int y)
{
    if ((x < _width && x >= 0) && (y < _height && y >= 0))
    {
        return _tiles[y][x];
    }
    return border;
}
//int enemies, int walls, int slows, int enemy_buldings, int enemy_towers, int chests
void map::generate(std::vector<int> parameters, std::vector<int> en_characts, std::vector<int> en_barr_characts, std::vector<int> en_tow_characts, player* pl)
{
    _tiles[pl->get_pos_y()][pl->get_pos_x()].status = PLAYER;
    _tiles[pl->get_pos_y()][pl->get_pos_x()].adress = player_ptr;
    gen_one(parameters[0], WALL);
    gen_one(parameters[1], SLOW);
    gen_one(parameters[2], CHEST);
    gen_one(parameters[3], ENEMY, 0, en_characts);
    gen_one(parameters[4], ENEMY_BARRACS, 1, en_barr_characts);
    gen_one(parameters[5], ENEMY_TOWER, 2, en_tow_characts);
}

void map::gen_one(int value, std::string stat)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> x(0, _width - 1);
    std::uniform_int_distribution<int> y(0, _height - 1);
    int rand_x, rand_y;
    for (int i = 0; i < value; i++)
    {
        rand_x = x(gen);
        rand_y = y(gen);
        if (_tiles[rand_y][rand_x].status == EMPTY)
        {
            _tiles[rand_y][rand_x].status = stat;
        }
        else
        {
            i--;
        }
    }
}

void map::gen_one(int value, std::string stat, int flag, std::vector<int> characteristics)
{
    static std::random_device rd;
    
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> x(0, _width - 1);
    std::uniform_int_distribution<int> y(0, _height - 1);
    int rand_x, rand_y;
    for (int i = 0; i < value; i++)
    {
        int rand_x = x(gen);
        int rand_y = y(gen);
        if (_tiles[rand_y][rand_x].status == EMPTY)
        {
            enemy* tmp_adr_enemy;
            enemy_barrack* tmp_adr_enemybarrack;
            enemy_tower* tmp_adr_enemytower;
            switch (flag)
            {
                case 0:
                    tmp_adr_enemy = new enemy(rand_x, rand_y, characteristics[0], characteristics[1], 0, 3);
                    add_enemy(tmp_adr_enemy, rand_x, rand_y);
                    break;
                
                case 1:
                    tmp_adr_enemybarrack = new enemy_barrack(rand_x, rand_y, characteristics[0]);
                    add_enemy_barrack(tmp_adr_enemybarrack, rand_x, rand_y);
                    break;

                case 2:
                    tmp_adr_enemytower = new enemy_tower(rand_x, rand_y, characteristics[0], characteristics[1], 3);
                    add_enemy_tower(tmp_adr_enemytower, rand_x, rand_y);
                    break;
            }
        }
        else
        {
            i--;
        }
    }
}

void map::render()
{
    for (int i = -1; i <= _width; i++)
    {
        if (i == -1)
        {
            std::cout << LEFT_UP_CORNER << UP_DOWN_WALL;
        }
        else if (i == _width)
        {
            std::cout << RIGHT_UP_CORNER << std::endl;
        }
        else
        {
            std::cout << UP_DOWN_WALL << UP_DOWN_WALL;
        }
    }
    for (int i = 0; i < _height; i++)
    {
        std::cout << RIGHT_LEFT_WALL << " ";
        for (int j = 0; j < _width; j++)
        {
            std::cout << _tiles[i][j].status << " ";
        }
        std::cout << RIGHT_LEFT_WALL << std::endl;
    }
    for (int i = -1; i <= _width; i++)
    {
        if (i == -1)
        {
            std::cout << LEFT_DOWN_CORNER << UP_DOWN_WALL;
        }
        else if (i == _width)
        {
            std::cout << RIGHT_DOWN_CORNER << std::endl;
        }
        else
        {
            std::cout << UP_DOWN_WALL << UP_DOWN_WALL;
        }
    }
}

std::vector<ally*> map::get_allies()
{
    return allies;
}

std::vector<enemy*> map::get_enemies()
{
    return enemies;
}

std::vector<enemy_barrack*> map::get_enemy_barracks()
{
    return barracks;
}

std::vector<enemy_tower*> map::get_enemy_towers()
{
    return towers;
}

std::vector<trap*> map::get_traps()
{
    return traps;
}

player* map::get_pl_ptr()
{
    return player_ptr;
}

void map::add_ally(ally* adr, int x, int y)
{
    _tiles[y][x].status = ALLY;
    _tiles[y][x].adress = adr;
    this->allies.push_back(adr);
}

void map::add_enemy(enemy* adr, int x, int y)
{
    _tiles[y][x].status = ENEMY;
    _tiles[y][x].adress = adr;
    this->enemies.push_back(adr);
}

void map::add_enemy_barrack(enemy_barrack* adr, int x, int y)
{
    _tiles[y][x].status = ENEMY_BARRACS;
    _tiles[y][x].adress = adr;
    this->barracks.push_back(adr);
}

void map::add_enemy_tower(enemy_tower* adr, int x, int y)
{
    _tiles[y][x].status = ENEMY_TOWER;
    _tiles[y][x].adress = adr;
    this->towers.push_back(adr);
}

void map::add_trap(trap* adr, int x, int y)
{
    _tiles[y][x].status = TRAP;
    _tiles[y][x].adress = adr;
    this->traps.push_back(adr);
}

void map::del_ally(ally* al)
{
    auto it = std::remove(allies.begin(), allies.end(), al);
    int x = al->get_pos_x();
    int y = al->get_pos_y();
    _tiles[y][x].status = EMPTY;
    _tiles[y][x].adress = nullptr;
    delete al;
    allies.erase(it, allies.end());
}

void map::del_enemy(enemy* en)
{
    auto it = std::remove(enemies.begin(), enemies.end(), en);
    int x = en->get_pos_x();
    int y = en->get_pos_y();
    _tiles[y][x].status = EMPTY;
    _tiles[y][x].adress = nullptr;
    delete en;
    enemies.erase(it, enemies.end());
}

void map::del_enemy_barrack(enemy_barrack* en_build)
{
    auto it = std::remove(barracks.begin(), barracks.end(), en_build);
    int x = en_build->get_pos_x();
    int y = en_build->get_pos_y();
    _tiles[y][x].status = EMPTY;
    _tiles[y][x].adress = nullptr;
    delete en_build;
    barracks.erase(it, barracks.end());
}

void map::del_enemy_tower(enemy_tower* en_tower)
{
    auto it = std::remove(towers.begin(), towers.end(), en_tower);
    int x = en_tower->get_pos_x();
    int y = en_tower->get_pos_y();
    _tiles[y][x].status = EMPTY;
    _tiles[y][x].adress = nullptr;
    delete en_tower;
    towers.erase(it, towers.end());
}

void map::del_trap(trap* tr)
{
    auto it = std::remove(traps.begin(), traps.end(), tr);
    int x = tr->get_pos_x();
    int y = tr->get_pos_y();
    _tiles[y][x].status = EMPTY;
    _tiles[y][x].adress = nullptr;
    delete tr;
    traps.erase(it, traps.end());
}

void map::replace_tile(std::string ch, int x, int y, int dx, int dy)
{
    _tiles[y][x].status = EMPTY;
    _tiles[y + dy][x + dx].status = ch;
}

void map::replace_tile(std::string ch, int x, int y, int dx, int dy, dummy* adr)
{
    _tiles[y][x].status = EMPTY;
    _tiles[y + dy][x + dx].status = ch;
    _tiles[y][x].adress = nullptr;
    _tiles[y + dy][x + dx].adress = adr;
}

std::string map::save_parameters()
{
    std::string data;
    data += save_par_helper(_width, DATA_SEP);
    data += save_par_helper(_height, DATA_SEP2);
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            data += _tiles[y][x].status + DATA_SEP;
        }
        data += DATA_SEP2;
    }
    data += save_par_helper(enemies.size(), DATA_SEP);
    data += save_par_helper(barracks.size(), DATA_SEP);
    data += save_par_helper(towers.size(), DATA_SEP);
    data += save_par_helper(allies.size(), DATA_SEP);
    data += save_par_helper(traps.size(), DATA_SEP);
    data += DATA_SEP2;
    for (enemy* i : enemies)
    {
        data += i->save_parameters();
    }
    for (enemy_barrack* i : barracks)
    {
        data += i->save_parameters();
    }
    for (enemy_tower* i : towers)
    {
        data += i->save_parameters();
    }
    for (ally* i : allies)
    {
        data += i->save_parameters();
    }
    for (trap* i : traps)
    {
        data += i->save_parameters();
    }
    return data;
}

std::string map::save_par_helper(int par, char sep)
{
    return std::to_string(par) + sep;
}

map::~map()
{
    for (int i = 0; i < _height; i++)
    {
        delete _tiles[i];
    }
    delete[] _tiles;
}