#pragma once
#include "entity.hpp"
#include "fireball.hpp" 

class Field;
class EntityManager;
class Hero;

class AttackTower : public Entity {
public:
    int health = 200;
    int cooldown = 4;
    int current_cooldown = 0;
    Fireball fireball_spell;

    AttackTower(int id);

    AttackTower(const AttackTower&) = default;
    AttackTower& operator=(const AttackTower&) = default;
    
    AttackTower(AttackTower&&) noexcept = default;
    AttackTower& operator=(AttackTower&&) noexcept = default;
    
    bool in_range(int hx, int hy, int tx, int ty) const;
    void try_attack(Field& field, EntityManager& roster, Hero& player, int tower_x, int tower_y);
    void tick();
};