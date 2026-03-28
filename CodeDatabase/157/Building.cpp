#include "Building.h"
#include <iostream>

Building::Building(int x, int y, int hp, bool enemyBuilding)
        : x_(x), y_(y), hp_(hp), enemyBuilding_(enemyBuilding) {}

void Building::takeDamage(int d) {
    hp_ -= d;
    std::cout << "Building at ("<<x_<<","<<y_<<") took "<<d<<" dmg. HP="<<hp_<<"\n";
    if (hp_ <= 0) {
        std::cout << "Building at ("<<x_<<","<<y_<<") destroyed.\n";
    }
}
