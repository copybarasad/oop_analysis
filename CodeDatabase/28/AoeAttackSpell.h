#pragma once
#include <string>
#include <iostream>
#include "Spell.h"
#include "Gamefield.h"
#include "Enemy.h"
#include "Player.h"

class AOEAttack : public Spell {
    std::string n;
    int r;
    int dmg;
public:
    AOEAttack(const std::string& name, int radius, int damage) : n(name), r(radius), dmg(damage) {}
    const std::string& name() const override { return n; }
    int radius() const override { return r; }
    bool cast(Player& player, Gamefield& field, int target_x, int target_y) override;
    
    void upgrade() override;
    int get_power() const override;
    int get_type_id() const override { return 2; }

};