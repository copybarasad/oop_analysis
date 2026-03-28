#pragma once
#include "../utils/Point.h"

class Entity{
protected:
    Point coords;
    int lifes;
    int damage;
public:
    Entity();

    Point get_coords();
    void set_coords(int x, int y);
    void shift_left();
    void shift_right();
    void shift_down();
    void shift_up();
    int get_lifes();     
    void take_damage(int damage);     
    bool is_alive(); 
    int get_damage();

    ~Entity();
};
