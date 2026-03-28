#pragma once

#include <vector>
#include <memory>
#include "player.h"
#include "enemy.h"
#include "building.h"
#include "tower.h"
#include "ally.h"

using namespace std;

enum class CellType {
    EMPTY,
    WALL,
    SLOWDOWN,
    PLAYER,
    ENEMY,
    BUILD,
    TRAP
};

class Field {
private:
    int width;
    int height;
    vector<vector<CellType>> grid;
    unique_ptr<Player> player;
    vector<unique_ptr<Enemy>> enemies;
    vector<unique_ptr<Building>> buildings;
    vector<unique_ptr<Tower>> enemy_towers;
    vector<unique_ptr<Ally>> allies;
    int player_x;
    int player_y;

    
    void walls();
    void slowdown_cells();

    struct Trap {
    int x;
    int y;
    int damage;
    Trap(int xx, int yy, int d) : x(xx), y(yy), damage(d) {}
    };
    vector<Trap> traps;

    bool loading_from_save = false;
    int game_level = 1;
    
public:
    Field(int field_width, int field_height);
    
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;
    ~Field() = default;
    
    void create_player(int x, int y, int health, int close_dmg, int far_dmg);
    void create_enemy(int x, int y, int health, int damage);
    void create_building(int x, int y, int spawn_interval, int health);
    void create_tower(int x, int y, int dmg = 8, int rad = 3, int cd = 2);
    void create_ally(int x, int y, int hp = 30, int dmg = 15);
    void initialize_grid();
    
    void check_traps(int enemy_x, int enemy_y);
    void update_towers();
    void update_allies();
    
    bool move_player(int dx, int dy);
    bool is_player_slowed() const;
    void move_enemies();
    void create_buildings();
    void player_attack(int target_x, int target_y);
    void finish_turn();
    
    void display() const;
    bool is_game_over() const;
    void switch_player_mode();
    
    int get_player_score() const;
    int get_width() const;
    int get_height() const;
    CellType get_cell_type(int x, int y) const;

    bool is_valid_position(int x, int y) const;
    bool is_cell_occupied(int x, int y) const;
    Enemy* get_enemy_at(int x, int y);

    void player_use_spell(size_t spell_idx, int target_x, int target_y);
    const vector<unique_ptr<Building>>& get_buildings() const;

    void place_trap(int x, int y, int damage);

    Player* get_player();
    const Player* get_player() const;
    int get_player_x() const;
    int get_player_y() const;
    void set_player_position(int x, int y);

    const vector<unique_ptr<Enemy>>& get_enemies() const;
    const vector<unique_ptr<Tower>>& get_towers() const;

    void set_loading_from_save(bool v) { loading_from_save = v; }
    char get_tile_symbol(int x, int y) const;
    void set_tile_from_symbol(int x, int y, char c);

    int get_level() const;
    void set_level(int lvl);
    void advance_level();
    bool is_level_completed() const;
};