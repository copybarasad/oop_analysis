#pragma once

class Enemy {
private:
    int health;
    int damage;
    int position_x;
    int position_y;

public:
    Enemy(int init_health, int damage, int x, int y);
    
    void take_damage(int damage_amount);
    void set_position(int x, int y);
    
    bool is_alive() const;
    int get_health() const;
    int get_damage() const;
    int get_x() const;
    int get_y() const;
};