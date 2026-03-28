#include "dummy.h"

dummy::dummy(int x, int y)
    :pos_x(x), pos_y(y){}

int dummy::get_pos_x()
{
    return pos_x;
}

int dummy::get_pos_y()
{
    return pos_y;
}

void dummy::change_pos_x(int dx)
{
    pos_x += dx;
}

void dummy::change_pos_y(int dy)
{
    pos_y += dy;
}

dummy::~dummy(){}


dummy_creature::dummy_creature(int x, int y, int hp, int dmg): dummy(x, y)
{
    health = hp;
    damage = dmg;
}

int dummy_creature::get_hp()
{
    return health;
}

int dummy_creature::get_dmg()
{
    return damage;
}

void dummy_creature::set_hp(int hp)
{
    health = hp;
}

void dummy_creature::change_hp(int dmg)
{
    health += dmg;
}

void dummy_creature::change_dmg(int ddmg)
{
    damage += ddmg;
}

dummy_creature::~dummy_creature(){}
