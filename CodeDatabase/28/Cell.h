#pragma once
#include <iostream>

class Player;
class Enemy;
class EnemyBuilding;
class EnemyTower;

class Cell {
private:
    Player* player_on_cell = nullptr;
    Enemy* enemy_on_cell = nullptr;
    EnemyBuilding* building_on_cell = nullptr;
    EnemyTower* enemy_tower_on_cell = nullptr;
    bool passable = true;
    bool trap_on_cell = false;
    bool slow = false;
    int x_pos;
    int y_pos;
    int trap_damage = 0;

public:
    Cell(int x = 0, int y = 0 );

    void set_passable(bool value);
    void set_slow(bool value);
    void set_player(Player* player);
    void set_enemy(Enemy* enemy);
    void set_building(EnemyBuilding* building);
    void set_enemy_tower(EnemyTower* tower);
    void set_x(int x);
    void set_y(int y);

    bool is_passable() const;
    bool is_slow() const;
    bool is_occupied() const;
    bool is_occupied_by_player() const;
    bool is_trap() const;

    Player* get_player() const;
    Enemy* get_enemy() const;
    EnemyBuilding* get_building() const;

    int get_x() const;
    int get_y() const;

    void place_trap(int damage);
    int get_trap_damage();
    int get_trap_damage_value() const;
    char get_symbol() const;
};
