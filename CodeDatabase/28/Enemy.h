#pragma once
#include "Entity.h"
#include "Cell.h"
#include "Gamefield.h"

class Enemy : public Entity {
private:
    int damage;
    int score_defeat = 10;
public:
    Enemy(int hp, int dmg, int x, int y);

    int get_damage() const;
    int get_hp() const;
    void take_damage(int amount);

    void go_to(Cell& cell) override;
    void move_towards_player(Player& player, Gamefield& field);
};
