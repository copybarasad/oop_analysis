#pragma once
#include "../utils/Point.h"
#include <vector>
#include <string>

class Spell{
public:
    virtual std::vector<Point> get_dests(Point player_coords, char c) = 0;

    virtual int get_damage() = 0;
    virtual int get_range() = 0;
    virtual std::string get_type() = 0;
    virtual bool need_to_return() = 0;
    virtual bool draw() = 0;
};
