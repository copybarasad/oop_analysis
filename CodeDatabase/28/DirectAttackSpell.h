#pragma once
#include <string>
#include "Spell.h"
#include "Gamefield.h"
#include "Player.h"
#include "Enemy.h"

class Gamefield;
class Player;
class Cell;

class DirectAttack : public Spell {
    std::string n;
    int r;
    int dmg;

public:
    DirectAttack(const std::string& name, int radius, int damage) : n(name), r(radius), dmg(damage) {}

    const std::string& name() const override { return n; }
    int radius() const override { return r; }

    bool cast(Player& player, Gamefield& field, int target_x, int target_y) override;
    bool tower_attack(Entity& attacker, Gamefield& grid, Player& player);

    void upgrade();
    int get_power() const;
    int get_type_id() const override { return 1; }
};