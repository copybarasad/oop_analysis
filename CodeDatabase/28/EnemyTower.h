#pragma once
#include "Entity.h"
#include "DirectAttackSpell.h"

class EnemyTower : public Entity {
private:
    Cell* cell;
    int radius;
    int spell_cooldown = 3;
    int current_timer = spell_cooldown;
    DirectAttack TowerAreaDamage{"Tower thunder",  3, 8};

public:
    EnemyTower(Cell* cell, int radius);
    void try_attack(Player& player, Gamefield& field);

    int get_x() const;
    int get_y() const;
};
