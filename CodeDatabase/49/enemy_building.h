#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include <iostream>

class GameMap;

class EnemyBuilding
{
private:
    int x, y;
    int health;
    int spawn_interval;
    int spawn_cooldown;

    static constexpr int DEFAULT_HEALTH = 6;

public:
    EnemyBuilding(int start_x, int start_y, int interval);
    EnemyBuilding(const EnemyBuilding& other);
    
    int get_x() const;
    int get_y() const;
    int get_health() const;
    int get_spawn_interval() const;
    int get_spawn_cooldown() const;
    
    bool is_alive() const;
    void take_damage(int spell_damage);
    
    void update();
    void reset_cooldown();
    bool can_spawn() const;

    bool spawn_enemy(GameMap& map, int x, int y);
    bool spawn_enemy_near_building(GameMap& map);

    void save_binary(std::ostream& os) const;
    void load_binary(std::istream& is);
};

#endif