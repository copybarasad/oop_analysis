#include "Unit.h"
#include <iostream>

Unit::Unit(int x, int y, int hp, bool enemy)
        : x_(x), y_(y), hp_(hp), enemy_(enemy) {}

void Unit::takeDamage(int d) {
    hp_ -= d;
    std::cout << "Unit at ("<<x_<<","<<y_<<") took "<<d<<" dmg. HP="<<hp_<<"\n";
    if (hp_ <= 0) {
        std::cout << "Unit at ("<<x_<<","<<y_<<") died.\n";
    }
}
