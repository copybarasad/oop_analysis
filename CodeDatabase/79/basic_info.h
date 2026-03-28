#ifndef BASIC_INFO_H
#define BASIC_INFO_H
#include <iostream>


class BasicInfo{
public:
    BasicInfo(int maxHealth_, int health_,  int damage_, int x_, int y_, int points_);

    int get_maxHealth() const;

    int get_health() const;

    int get_x() const;
    int get_y() const;

    int get_damage() const;

    bool check_health();

    void damage_recieved(int dam);

    void print_current_stats();
    int get_points();
    
protected:
    int maxHealth;
    int health;
    int damage;
    int x;
    int y;
    int points;
};

#endif
