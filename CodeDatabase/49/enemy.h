#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <vector>

class GameMap;

class Enemy
{
private:
    int x, y;
    int health;
    int damage;

    static constexpr int DEFAULT_HEALTH = 6;
    static constexpr int DEFAULT_DAMAGE = 1;
    static constexpr int DAMAGE_BY_TRAP_SPELL = 2;

    bool find_next_move_dijkstra(GameMap& map, int start_x, int start_y, int target_x, int target_y,
        int& next_x, int& next_y);
    std::pair<int, int> reconstruct_first_move(int target_x, int target_y, int start_x, int start_y,
        const std::vector<std::vector<std::pair<int, int>>>& parent);
    
public:
    Enemy(int start_x, int start_y);

    int get_x() const;
    int get_y() const;
    int get_health() const;
    int get_damage() const;

    void set_position(int new_x, int new_y);
    bool is_alive() const;
    void take_damage(int player_damage);

    bool move(GameMap& map, size_t enemy_index);

    void save_binary(std::ostream& os) const;
    void load_binary(std::istream& is);
};

#endif