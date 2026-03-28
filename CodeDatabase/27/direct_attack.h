#pragma once
#include <string>
#include "ispell.h"

class Grid;
class Player;
class Unit;

class DirectAttack : public ISpell {
    std::string n;
    int r;
    int dmg;
    //metadata

private:
    bool in_radius_manhattan(int sx,int sy,int tx,int ty) const;

public:
    DirectAttack(const std::string& name, int radius, int damage) : n(name), r(radius), dmg(damage) {}
    //constructor
    SpellId id() const override { return SpellId::DirectAttack; }
    const std::string& name() const override { return n; }
    int radius() const override { return r; }
    int damage() const override { return dmg; }
    //more metadata
    bool cast(Player& player, Grid& grid, int target_x, int target_y) override;
    //переопределение cast
    bool tower_attack(Unit& attacker, Grid& grid, Player& player);
};
