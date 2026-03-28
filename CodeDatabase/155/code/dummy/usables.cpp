#include "usables.h"

trap::trap(int x, int y, int dmg) : dummy(x, y)
{
    damage = dmg;
}

void trap::make_turn(map*){}

int trap::got_hit(int)
{
    return damage;
}

std::string trap::save_parameters()
{
    std::string data;
    data += save_par_helper(get_pos_x(), DATA_SEP);
    data += save_par_helper(get_pos_y(), DATA_SEP);
    data += save_par_helper(damage, DATA_SEP2);
    return data;
}

std::string trap::save_par_helper(int par, char sep)
{
    return std::to_string(par) + sep;
}

trap::~trap(){}