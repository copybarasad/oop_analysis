#pragma once
#include <memory>
#include <string>

class Field;
class Player;

class Tower {
private:
    int x;
    int y;
    int attack_damage;
    int attack_radius;
    int cooldown;
    int current_cooldown;

public:
    Tower(int pos_x, int pos_y, int dmg = 10, int rad = 3, int cd = 5);

    void update();
    void try_attack(Field& field);

    int get_x() const;
    int get_y() const;
    
    int get_damage() const;
    int get_radius() const;
    int get_cooldown() const;
    int get_current_cooldown() const;
    void set_current_cooldown(int new_cooldown);

    bool is_active() const;

    std::string get_symbol() const;
};