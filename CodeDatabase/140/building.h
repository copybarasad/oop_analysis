#pragma once
class Building {
private:
    int position_x;
    int position_y;
    int creating_interval;
    int steps_until_create;
    int health;

public:
    Building(int x, int y, int interval, int init_health);
    
    virtual ~Building() = default;

    void did_step();
    bool do_create_enemy() const;
    void reset_timer();
    
    int get_x() const;
    int get_y() const;
    int get_interval() const;

    void take_damage(int damage_amount);
    bool is_destroyed() const;
    int get_health() const;
};