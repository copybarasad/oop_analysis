#pragma once

#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <algorithm>

#include "cell.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy_build.hpp"
#include "enemy_tower.hpp"

#define MIN_SIZE 10
#define MAX_SIZE 25

class World{
private:
    int size_x, size_y;
    std::vector<std::vector<Cell>> cells;
    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<EnemyBuild*> enemies_build;
    std::vector<EnemyTower*> enemies_tower;

    int validate_coord(int coord);

public:
    World(int size_x = MIN_SIZE, int size_y = MIN_SIZE, Player* player_ptr = nullptr);
    World(int size_x, int size_y, Player* player_ptr,
        std::vector<Enemy*> enemies, std::vector<EnemyBuild*> enemies_build, std::vector<EnemyTower*> enemies_tower);
    World(const World& other);
    World& operator=(const World& other);
    World(World&& other);
    World& operator=(World&& other);

    ~World();

    int random(int max_size);
    bool is_valid_coord(int x, int y);

    int get_size_x();
    int get_size_y();
    void get_sizes(int& size_x, int& size_y);

    Cell& get_cell(int x, int y);
    Cell* get_cell(Entity* entity);

    void set_cells(std::vector<std::vector<Cell>> cells);
    void set_enemies(std::vector<Enemy*> enemies);
    void set_enemies_build(std::vector<EnemyBuild*> enemies_build);
    void set_enemies_tower(std::vector<EnemyTower*> enemies_tower);

    void set_player(Player* player_ptr);

    int add_enemy(int x, int y, Enemy* enemy);
    void gen_eneimes(int cnt_enemies, float coef);
    void gen_enemies_build(int cnt_enemies_build, int required_quantity, float coef);
    void gen_enemies_tower(int cnt_enemies_tower, float coef);
    std::vector<Enemy*> get_enemies();
    std::vector<EnemyBuild*> get_enemies_build();
    std::vector<EnemyTower*> get_enemies_tower();

    int get_cur_cooldown_e_build();
    void push_enemy_to_vector(Enemy* enemy);

    int move_player(int x, int y);
    Player& get_player();

    int move_enemy(int x, int y, Enemy* enemy);

    TypeCell gen_state_cell();

    bool magic_damage(int damage, Cell* target_cell);

    bool has_eny_enemies();

    void print();
};