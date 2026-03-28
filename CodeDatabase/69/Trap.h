#pragma once
#include "Position.h"
#include <string>

class Trap {
public:
    Trap(const std::string& n, Position p, int d, bool active);
    std::string name;
    Position pos;
    int damage;
    bool active;
};