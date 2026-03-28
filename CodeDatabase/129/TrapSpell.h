#pragma once
#include <vector>
#include <string>
#include "../utils/Values.h"
#include "Spell.h"

class TrapSpell : public Spell{
private:
    std::string type;
    int damage;
    int range;
public:
    TrapSpell();

    std::vector<Point> get_dests(Point player_coords, char c);
    int get_damage();
    int get_range();
    std::string get_type();
    bool need_to_return();
    bool draw();

    ~TrapSpell() ;
};
