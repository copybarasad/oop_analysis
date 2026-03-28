#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <memory>
#include <string>
#include "cell.h"
#include "player.h"
#include "enemy.h"
#include "enemytower.h"
#include "trap.h"

class Field {
private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
    std::unique_ptr<Player> player_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<EnemyTower>> enemy_towers_;
    std::vector<Trap> traps_;

public:
    Field(int width, int height);

    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    void set_player(std::unique_ptr<Player> player);
    bool move_player(int dx, int dy);
    void add_enemy(std::unique_ptr<Enemy> enemy);
    void add_enemy_tower(std::unique_ptr<EnemyTower> tower);
    void move_enemies();
    void update_towers();
    bool player_attack(int direction_x, int direction_y);

    bool is_game_over() const;
    bool is_valid_position(int x, int y) const;
    bool can_move_to(int x, int y) const;
    
    bool has_enemy_at(int x, int y) const;
    bool has_enemy_tower_at(int x, int y) const;
    bool has_trap_at(int x, int y) const;
    void place_trap(int x, int y, int damage);
    void check_traps();

    int get_width() const;
    int get_height() const;
    const Cell& get_cell(int x, int y) const;
    const Player* get_player() const;
    const std::vector<std::unique_ptr<Enemy>>& get_enemies() const;
    const std::vector<std::unique_ptr<EnemyTower>>& get_enemy_towers() const;
    void print_enemies_info() const;
    void print_towers_info() const;

    void save_to_file(const std::string& filename) const;
    void load_from_file(const std::string& filename);

    bool are_all_enemies_dead() const;
    std::unique_ptr<Player> extract_player();

private:
    void initialize_grid();
    void copy_from(const Field& other);
    void generate_walls();
};

#endif