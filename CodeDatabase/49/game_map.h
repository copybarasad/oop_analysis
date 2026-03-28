#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <iomanip>
#include <algorithm>
#include "cell.h"
#include "../player/player.h"
#include "../enemies/enemy.h"
#include "../enemies/enemy_building.h"
#include "../enemies/enemy_tower.h"
#include "../spells/direct_damage_spell.h"
#include "../spells/area_damage_spell.h"
#include "../spells/trap_spell.h"

class GameMap
{
private:
    int width, height;
    int level;
    std::vector<std::vector<Cell>> cells;
    Player* player;
    std::vector<Enemy> enemies;
    EnemyTower* enemy_tower;
    EnemyBuilding* enemy_building;

    static constexpr int MIN_MAP_WIDTH = 10;
    static constexpr int MAX_MAP_WIDTH = 25;
    static constexpr int MIN_MAP_HEIGHT = 10;
    static constexpr int MAX_MAP_HEIGHT = 25;

    static constexpr int INITIAL_ENEMY_COUNT = 3;
    static constexpr int INITIAL_BUILDING_COUNT = 1;
    static constexpr int INITIAL_TOWER_COUNT = 1;
    static constexpr int WALL_PROBABILITY = 15;
    static constexpr int TRAP_PROBABILITY = 10;

    static constexpr int BUILDING_SPAWN_INTERVAL = 15;

    static constexpr int MIN_SPAWN_DISTANCE_FROM_PLAYER = 3;
    static constexpr int MIN_BUILDING_DISTANCE_FROM_PLAYER = 5;
    static constexpr int MIN_BUILDING_DISTANCE_FROM_TOWER = 7;
    static constexpr int MIN_TOWER_DISTANCE_FROM_PLAYER = 7;

    void initialize_cells();

    void generate_enemies();
    void generate_enemy_building();
    void generate_enemy_tower();
    void generate_walls();
    void generate_slow_traps();

public:
    GameMap(int w, int h);
    GameMap(int w, int h, int level);
    GameMap(int w, int h, int level, Player* existing_player);
    GameMap(const GameMap& other);
    GameMap(GameMap&& other) noexcept;
    GameMap& operator=(const GameMap& other);
    GameMap& operator=(GameMap&& other) noexcept;
    ~GameMap();

    int get_width() const;
    int get_height() const;
    Player* get_player() const;
    const std::vector<Enemy>& get_enemies() const;
    std::vector<Enemy>& get_enemies_mutable();
    EnemyTower* get_enemy_tower() const;
    EnemyBuilding* get_enemy_building() const;
    Cell& get_cell(int x, int y);
    const Cell& get_cell(int x, int y) const;
    const std::vector<std::vector<Cell>>& get_cells() const;

    bool is_valid_position(int x, int y) const;

    void remove_dead_enemies();

    void display() const;

    void save_binary(std::ostream& os) const;
    void load_binary(std::istream& is);
};

#endif