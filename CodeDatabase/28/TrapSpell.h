#pragma once
#include <string>
#include "Spell.h"

class Trap : public Spell {
    std::string n;
    int r;
    int dmg;

public:
    Trap(const std::string& name, int radius, int damage) : n(name), r(radius), dmg(damage) {}

    const std::string& name() const override { return n; }
    int radius() const override { return r; }

    bool cast(Player& player, Gamefield& grid, int target_x, int target_y);

    void upgrade();
    int get_power() const;
    int get_type_id() const override { return 3; }
};
