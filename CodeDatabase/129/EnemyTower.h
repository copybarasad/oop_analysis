#pragma once
#include <vector>
#include <string>
#include "../utils/Values.h"
#include "../spells/DDSpell.h"


class EnemyTower{
private:
    int regenerate_moves;
    Point coords;
    DDSpell dd_spell;
public:
    EnemyTower() ;
    EnemyTower(Point pos);
    ~EnemyTower();

    std::vector<Point> get_dests();

    int get_regenerate_moves();
    int get_damage();
    Point get_coords();

    bool time_to_kill();

    void wait_moves();
    void dont_wait_moves();

    void reset_regenerate_moves();
    void set_regenerate_moves();
};
