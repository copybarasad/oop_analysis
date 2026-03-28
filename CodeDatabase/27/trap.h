#pragma once
#include <string>
#include "ispell.h"

class Trap : public ISpell {
    std::string n;
    int r;
    int dmg;
    //metadata

private:
    bool in_radius_manhattan(int sx,int sy,int tx,int ty) const;
    
public:
    Trap(const std::string& name, int radius, int damage)
        : n(name), r(radius), dmg(damage) {}
    //constructor
    SpellId id() const override { return SpellId::Trap; }
    const std::string& name() const override { return n; }
    int radius() const override { return r; }
    int damage() const override { return dmg; }
    //more metadata
    bool cast(Player& player, Grid& grid, int target_x, int target_y) override;
    //переопределение cast
};
