#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "player.h"
#include "playing_field.h"
#include <vector>
#include <memory>

class Enemy;

class EnemyTower {
private:
    int position_x;
    int position_y;
    int health;
    int max_health;
    int attack_range;
    int attack_cooldown;
    int current_cooldown;
    int damage;
    bool can_attack_this_turn;
    bool alive; 

public:
    EnemyTower(int x, int y);
    
    void update(Player& player, const Playing_field& field);
    bool attack(Player& player, const Playing_field& field);
    void take_damage(int damage);
    
    int get_position_x() const;
    int get_position_y() const;
    bool get_can_attack() const;
    int get_attack_range() const;
    int get_health() const;
    int get_max_health() const;
    bool is_alive() const; 

    void set_health(int health);
    void set_max_health(int max_health);
    void set_alive(bool alive);
};

#endif