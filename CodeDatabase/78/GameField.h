#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <memory>

#include "Cell.h"

#include "Player.h"
#include "Enemy.h"
#include "Tower.h"
#include "Ally.h"
#include "ISpell.h"

class GameField {
public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other);
    
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other); 

    ~GameField();
    
    Cell& get_cell(int x, int y);
    int get_width() const { return width_; }
    int get_height() const { return height_; }
    
    std::unique_ptr<Player> extract_player();
    Player* get_player() const { return player_.get(); }

    const std::vector<std::unique_ptr<Enemy>>& get_enemies() const { return enemies_; }
    const std::vector<std::unique_ptr<Tower>>& get_towers() const { return towers_; }
    const std::vector<std::unique_ptr<Ally>>& get_allies() const { return allies_; }

    bool is_coordinates_valid(int x, int y) const;
    bool is_cell_occupied(int x, int y) const;
    bool is_cell_passable(int x, int y) const;
    bool is_cell_empty(int x, int y) const;

    bool place_entity(std::unique_ptr<Entity> entity, int x, int y);
    bool move_entity(int from_x, int from_y, int to_x, int to_y);

    void spawn_random_enemies(int count, int default_health = 50);
    void spawn_random_towers(int count, int default_health = 100, int default_r_s = 2, int default_m_c = 10, int health_spawned_enemy = 50, float multiplier = 0.2);
    void spawn_random_cell_type(int count, CellType cell_type);
    
    void update();
    void update_enemies(int& total_gold_reward, int& total_score_reward);
    void update_towers(int& total_gold_reward, int& total_score_reward);
    void update_allies();
    
    void draw_field() const;
    void show_enemy_stats() const;

    std::string get_cell_symbol(int x, int y) const;
    std::string serialize() const;
    
private:
    int width_;
    int height_;
    Cell** cells_;
    std::unique_ptr<Player> player_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Tower>> towers_;
    std::vector<std::unique_ptr<Ally>> allies_;
    
    bool are_dimensions_valid(int width, int height);
    void set_dimensions(int width, int height);
    void initialize_cells();
    void cleanup_cells();
    void copy_from(const GameField& other);
    void move_from(GameField&& other);
};

#endif