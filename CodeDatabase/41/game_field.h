#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "cell.h"
#include "constants.h"
#include <vector>
#include <memory>
#include "enums.h"
#include <string>

class Player;
class Enemy;
class Building;
class Trap;
class Ally;

class Game_field{
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> cells;
    int player_x;
    int player_y;
    std::vector<std::pair<int, int>> enemy_positions;
    std::vector<std::unique_ptr<Building>> buildings;
    std::vector<Trap> traps; 

    Player* player; 
    std::vector<std::unique_ptr<Enemy>> enemies;

    //союзники
    std::vector<std::unique_ptr<Ally>> allies;           
    std::vector<std::pair<int, int>> ally_positions;

public:
    Game_field(int w, int h, Player* player_ref);
    Game_field(const Game_field& other);
    Game_field(Game_field&& other) noexcept;
    Game_field& operator=(const Game_field& other);
    Game_field& operator=(Game_field&& other) noexcept;
    ~Game_field();

    int get_width() const;
    int get_height() const;
    Cell get_cell(int x, int y) const;
    std::pair<int, int> get_player_position() const;
    std::vector<std::pair<int, int>> get_enemy_positions() const;
    std::pair<int, int> get_enemy_position(const Enemy* enemy) const;
    int find_enemy_index(const Enemy* enemy) const;
    size_t get_enemy_count() const { return enemies.size(); }
    Player* get_player() const { return player; }

    void initialize_field();
    bool place_player(int x, int y);
    void add_enemy(Enemy* enemy, int x, int y);
    void add_building(const std::string& name, int x, int y, int spawn_interval = SPAWN_INTERVAL);
    void update_enemies();

    Move_status move_player(int delta_x, int delta_y);
    bool move_enemy(int enemy_index, int new_x, int new_y);
    bool is_valid_position(int x, int y) const;
    bool can_attack(int attacker_x, int attacker_y, int target_x, int target_y, Combat_mode mode) const;
    void perform_attack(int attaker_x, int attacker_y, int target_x, int target_y);
    void update_buildings();
    void display_field() const;

    //заклинания
    bool is_enemy_or_building_at(int x, int y) const;
    void perform_spell_attack(const std::string& attaker_name, int target_x, int target_y, int damage);
    void perform_area_attack(const std::string& attaker_name, int center_x, int center_y, int damage, int area_size);

    //здания
    Building* get_building_at(int x, int y) const;
    void destroy_building(size_t index);
    //вражеские
    void add_enemy_tower(const std::string& name, int x, int y);

    //ловушки
    void damage_enemy_at_position(int x, int y, int damage);
    void place_trap(int x, int y, int damage);  
    void update_traps();  
    const std::vector<Trap>& get_traps() const; 

    //союзники
    void add_ally(Ally* ally, int x, int y);
    void update_allies();
    std::pair<int, int> get_ally_position(const Ally* ally) const;
    bool move_ally(Ally* ally, int new_x, int new_y);

    double calculate_distance(int x1, int y1, int x2, int y2) const;

    void set_cell_type(int x, int y, Cell_type type);

private:
    void copy_from(const Game_field& other);
    void move_from(Game_field&& other) noexcept;
    void clear_field();
    bool is_cell_blocked(int x, int y) const;
};

#endif